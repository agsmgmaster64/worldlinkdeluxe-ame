#include "global.h"
#include "main.h"
#include "battle.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "field_screen_effect.h"
#include "graphics.h"
#include "gpu_regs.h"
#include "list_menu.h"
#include "move_relearner_rg.h"
#include "malloc.h"
#include "menu.h"
#include "menu_specialized.h"
#include "move.h"
#include "move_relearner.h"
#include "overworld.h"
#include "pokemon_summary_screen.h"
#include "palette.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "trig.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/moves.h"

/*
 * Move relearner state machine
 * ------------------------
 * 
 * CB2_InitLearnMove_RG
 *   - Creates listMenuScrollPos to listen to right/left buttons.
 *   - Creates listMenuScrollRow to listen to up/down buttons.
 * DoMoveRelearnerMain: MENU_STATE_FADE_TO_BLACK
 * DoMoveRelearnerMain: MENU_STATE_WAIT_FOR_FADE
 *   - Go to MENU_STATE_IDLE_BATTLE_MODE
 * 
 * DoMoveRelearnerMain: MENU_STATE_SETUP_BATTLE_MODE
 * DoMoveRelearnerMain: MENU_STATE_IDLE_BATTLE_MODE
 *   - If the player selected a move (pressed A), go to MENU_STATE_PRINT_TEACH_MOVE_PROMPT.
 *   - If the player cancelled (pressed B), go to MENU_STATE_PRINT_GIVE_UP_PROMPT.
 * 
 * DoMoveRelearnerMain: MENU_STATE_PRINT_TEACH_MOVE_PROMPT
 * DoMoveRelearnerMain: MENU_STATE_TEACH_MOVE_CONFIRM
 *   - Wait for the player to confirm.
 *   - If cancelled, go to MENU_STATE_SETUP_BATTLE_MODE.
 *   - If confirmed and the pokemon had an empty move slot, set VAR_0x8004 to TRUE and go to
 *     MENU_STATE_PRINT_TEXT_THEN_FANFARE.
 *   - If confirmed and the pokemon doesn't have an empty move slot, go to
 *     MENU_STATE_PRINT_TRYING_TO_LEARN_PROMPT.
 * 
 * DoMoveRelearnerMain: MENU_STATE_PRINT_TRYING_TO_LEARN_PROMPT
 * DoMoveRelearnerMain: MENU_STATE_WAIT_FOR_TRYING_TO_LEARN
 * DoMoveRelearnerMain: MENU_STATE_CONFIRM_DELETE_OLD_MOVE
 *   - If the player confirms, go to MENU_STATE_PRINT_WHICH_MOVE_PROMPT.
 *   - If the player cancels, go to MENU_STATE_PRINT_STOP_TEACHING
 * 
 * DoMoveRelearnerMain: MENU_STATE_PRINT_STOP_TEACHING
 * DoMoveRelearnerMain: MENU_STATE_WAIT_FOR_STOP_TEACHING
 * DoMoveRelearnerMain: MENU_STATE_CONFIRM_STOP_TEACHING
 *   - If the player confirms, go to MENU_STATE_CHOOSE_SETUP_STATE.
 *   - If the player cancels, go back to MENU_STATE_PRINT_TRYING_TO_LEARN_PROMPT.
 * 
 * DoMoveRelearnerMain: MENU_STATE_PRINT_WHICH_MOVE_PROMPT
 * DoMoveRelearnerMain: MENU_STATE_SHOW_MOVE_SUMMARY_SCREEN
 *   - Go to ShowSelectMovePokemonSummaryScreen. When done, control returns to
 *     CB2_InitLearnMoveReturnFromSelectMove_RG.
 * 
 * DoMoveRelearnerMain: MENU_STATE_DOUBLE_FANFARE_FORGOT_MOVE
 * DoMoveRelearnerMain: MENU_STATE_PRINT_TEXT_THEN_FANFARE
 * DoMoveRelearnerMain: MENU_STATE_WAIT_FOR_FANFARE
 * DoMoveRelearnerMain: MENU_STATE_WAIT_FOR_A_BUTTON
 * DoMoveRelearnerMain: MENU_STATE_FADE_AND_RETURN
 * DoMoveRelearnerMain: MENU_STATE_RETURN_TO_FIELD
 *   - Clean up and go to CB2_ReturnToField.
 * 
 * DoMoveRelearnerMain: MENU_STATE_PRINT_GIVE_UP_PROMPT
 * DoMoveRelearnerMain: MENU_STATE_GIVE_UP_CONFIRM
 *   - If the player confirms, go to MENU_STATE_FADE_AND_RETURN, and set VAR_0x8004 to FALSE.
 *   - If the player cancels, go to MENU_STATE_SETUP_BATTLE_MODE.
 * 
 * CB2_InitLearnMoveReturnFromSelectMove_RG:
 *   - Do most of the same stuff as CB2_InitLearnMove_RG.
 * DoMoveRelearnerMain: MENU_STATE_FADE_FROM_SUMMARY_SCREEN
 * DoMoveRelearnerMain: MENU_STATE_TRY_OVERWRITE_MOVE
 *   - If any of the pokemon's existing moves were chosen, overwrite the move and
 *     go to MENU_STATE_DOUBLE_FANFARE_FORGOT_MOVE and set VAR_0x8004 to TRUE.
 *   - If the chosen move is the one the player selected before the summary screen,
 *     go to MENU_STATE_PRINT_STOP_TEACHING.
 * 
 */
 
#define MENU_STATE_FADE_TO_BLACK 0
#define MENU_STATE_WAIT_FOR_FADE 1
#define MENU_STATE_UNREACHABLE 2
#define MENU_STATE_SETUP_BATTLE_MODE 3
#define MENU_STATE_IDLE_BATTLE_MODE 4
// States 5, 6, and 7 are skipped.
#define MENU_STATE_PRINT_TEACH_MOVE_PROMPT 8
#define MENU_STATE_TEACH_MOVE_CONFIRM 9
// States 10 and 11 are skipped.
#define MENU_STATE_PRINT_GIVE_UP_PROMPT 12
#define MENU_STATE_GIVE_UP_CONFIRM 13
#define MENU_STATE_FADE_AND_RETURN 14
#define MENU_STATE_RETURN_TO_FIELD 15
#define MENU_STATE_PRINT_TRYING_TO_LEARN_PROMPT 16
#define MENU_STATE_WAIT_FOR_TRYING_TO_LEARN 17
#define MENU_STATE_CONFIRM_DELETE_OLD_MOVE 18
#define MENU_STATE_PRINT_WHICH_MOVE_PROMPT 19
#define MENU_STATE_SHOW_MOVE_SUMMARY_SCREEN 20
// States 21, 22, and 23 are skipped.
#define MENU_STATE_PRINT_STOP_TEACHING 24
#define MENU_STATE_WAIT_FOR_STOP_TEACHING 25
#define MENU_STATE_CONFIRM_STOP_TEACHING 26
#define MENU_STATE_CHOOSE_SETUP_STATE 27
#define MENU_STATE_FADE_FROM_SUMMARY_SCREEN 28
#define MENU_STATE_TRY_OVERWRITE_MOVE 29
#define MENU_STATE_DOUBLE_FANFARE_FORGOT_MOVE 30
#define MENU_STATE_PRINT_TEXT_THEN_FANFARE 31
#define MENU_STATE_WAIT_FOR_FANFARE 32
#define MENU_STATE_WAIT_FOR_A_BUTTON 33

enum {
    RELEARN_WINDOW_MOVE_ICONS_1,
    RELEARN_WINDOW_MOVE_ICONS_2,
    RELEARN_WINDOW_MOVE_ICONS_3,
    RELEARN_WINDOW_MOVE_STATS,
    RELEARN_WINDOW_MOVE_DESCRIPTION,
    RELEARN_WINDOW_MOVE_LIST,
    RELEARN_WINDOW_MESSAGE,
    RELEARN_WINDOW_COUNT,
};

enum {
    RELEARN_COLOR_NORMAL,
    RELEARN_COLOR_NORMAL_SKIP,
    RELEARN_COLOR_ALT,
};

struct LearnMoveGfxResources
{
    u8 state;
    u8 numMenuChoices;
    bool8 scheduleMoveInfoUpdate;
    u8 partyMon;
    u8 moveSlot;
    u8 numToShowAtOnce;
    u8 moveListMenuTask;
    s8 selectedIndex;
    u8 bg1TilemapBuffer[BG_SCREEN_SIZE]; // must be 4-byte aligned to work properly, which is why this struct has been reordered
    struct ListMenuItem menuItems[MAX_RELEARNER_MOVES + 1];
    u16 movesToLearn[MAX_RELEARNER_MOVES];
    u8 moveListScrollArrowTask;
    u16 listMenuScrollPos;
    u16 listMenuScrollRow;
};

static EWRAM_DATA struct LearnMoveGfxResources * sMoveRelearner = NULL;

static void CB2_MoveRelearner(void);
static void DoMoveRelearnerMain(void);
static void DrawTextBorderOnListAndMessage(void);
static void PrintTeachWhichMoveToStrVar1(bool8 onInit);
static void InitMoveRelearnerStateVariables(void);
static void MoveRelearnerInitListMenuBuffersEtc(void);
static void MoveRelearnerMenuHandleInput(void);
static void MoveLearnerInitListMenu(void);
static void LoadMoveInfoUI(void);
static void PrintMoveInfoHandleCancel_CopyToVram(void);
static void MoveRelearnerMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static s8 YesNoMenuProcessInput(void);
static void PrintTextOnWindow(u8 windowId, const u8 *str, u8 x, u8 y, s32 speed, s32 colorIdx);

static const u16 sMoveRelearner_Pal[] = INCBIN_U16("graphics/move_relearner_rg/learn_move.gbapal");
static const u32 sMoveRelearner_Gfx[] = INCBIN_U32("graphics/move_relearner_rg/learn_move.4bpp.lz");
static const u32 sMoveRelearner_Tilemap[] = INCBIN_U32("graphics/move_relearner_rg/learn_move.bin.lz");

static const struct BgTemplate sBgTemplates[2] = {
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .priority = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 8,
        .priority = 1
    }
};

static const struct WindowTemplate sWindowTemplates[] =
{
    [RELEARN_WINDOW_MOVE_ICONS_1] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 6,
        .height = 6,
        .paletteNum = 13,
        .baseBlock = 0x014
    },
    [RELEARN_WINDOW_MOVE_ICONS_2] =
    {
        .bg = 0,
        .tilemapLeft = 10,
        .tilemapTop = 0,
        .width = 5,
        .height = 6,
        .paletteNum = 13,
        .baseBlock = 0x039
    },
    [RELEARN_WINDOW_MOVE_ICONS_3] =
    {
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 0,
        .width = 5,
        .height = 4,
        .paletteNum = 13,
        .baseBlock = 0x057
    },
    [RELEARN_WINDOW_MOVE_STATS] =
    {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 0,
        .width = 3,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 0x06C
    },
    [RELEARN_WINDOW_MOVE_DESCRIPTION] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 6,
        .width = 15,
        .height = 8,
        .paletteNum = 15,
        .baseBlock = 0x07E
    },
    [RELEARN_WINDOW_MOVE_LIST] =
    {
        .bg = 0,
        .tilemapLeft = 19,
        .tilemapTop = 1,
        .width = 10,
        .height = 12,
        .paletteNum = 15,
        .baseBlock = 0x0F6
    },
    [RELEARN_WINDOW_MESSAGE] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x16E
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sRelearnTextColors[][3] =
{
    [RELEARN_COLOR_NORMAL]         = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
    [RELEARN_COLOR_NORMAL_SKIP]    = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
    [RELEARN_COLOR_ALT]            = {TEXT_COLOR_WHITE,       TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
};

static const struct WindowTemplate sMoveRelearnerYesNoMenuTemplate =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 8,
    .width = 6,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x1d1
};

static const struct ListMenuTemplate sMoveRelearnerListMenuTemplate =
{
    .items = NULL,
    .moveCursorFunc = MoveRelearnerMenu_MoveCursorFunc,
    .itemPrintFunc = NULL,
    .totalItems = 0,
    .maxShowed = 6,
    .windowId = RELEARN_WINDOW_MOVE_LIST,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = FONT_NORMAL,
    .cursorKind = CURSOR_BLACK_ARROW,
};

static void VBlankCB_MoveRelearner(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MoveRelearnerLoadBgGfx(void)
{
    int i;
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    ResetTempTileDataBuffers();
    if (InitWindows(sWindowTemplates))
    {
        DeactivateAllTextPrinters();
        for (i = 0; i < ARRAY_COUNT(sWindowTemplates); i++)
        {
            ClearWindowTilemap(i);
            FillWindowPixelBuffer(i, PIXEL_FILL(0));
        }
        FillWindowPixelBuffer(RELEARN_WINDOW_MESSAGE, PIXEL_FILL(1));
        FillBgTilemapBufferRect(0, 0x000, 0, 0, 30, 20, 15);
        SetBgTilemapBuffer(1, sMoveRelearner->bg1TilemapBuffer);
        LoadUserWindowBorderGfx(RELEARN_WINDOW_MOVE_ICONS_1, 1, BG_PLTT_ID(14));
        ListMenuLoadStdPalAt(BG_PLTT_ID(13), 1);
        LoadPalette(sMoveRelearner_Pal, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        DecompressAndLoadBgGfxUsingHeap(1, sMoveRelearner_Gfx, 0, 0, 0);
        CopyToBgTilemapBuffer(1, sMoveRelearner_Tilemap, 0, 0);
        CopyBgTilemapBufferToVram(1);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    }
}

void CB2_InitLearnMove_RG(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetTasks();
    sMoveRelearner = AllocZeroed(sizeof(struct LearnMoveGfxResources));
    InitMoveRelearnerStateVariables();
    sMoveRelearner->partyMon = gSpecialVar_0x8004;
    MoveRelearnerInitListMenuBuffersEtc();
    SetVBlankCallback(VBlankCB_MoveRelearner);
    MoveRelearnerLoadBgGfx();
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    SetMainCallback2(CB2_MoveRelearner);
}

static void CB2_InitLearnMoveReturnFromSelectMove_RG(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetTasks();
    MoveRelearnerInitListMenuBuffersEtc();
    sMoveRelearner->moveSlot = gSpecialVar_0x8005;
    SetVBlankCallback(VBlankCB_MoveRelearner);
    MoveRelearnerLoadBgGfx();
    SetBackdropFromColor(RGB_BLACK);

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    SetMainCallback2(CB2_MoveRelearner);
}

static void CB2_MoveRelearner(void)
{
    if (!IsTextPrinterActive(RELEARN_WINDOW_MESSAGE))
        DoMoveRelearnerMain();
    if (sMoveRelearner->scheduleMoveInfoUpdate)
    {
        PrintMoveInfoHandleCancel_CopyToVram();
        sMoveRelearner->scheduleMoveInfoUpdate = FALSE;
    }
    RunTasks();
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void MoveRelearnerExpandAndPrintMessage(const u8 *str)
{
    StringExpandPlaceholders(gStringVar4, str);
    PrintTextOnWindow(RELEARN_WINDOW_MESSAGE, gStringVar4, 0, 2, GetPlayerTextSpeedDelay(), RELEARN_COLOR_ALT);
}

static void DoMoveRelearnerMain(void)
{
    u16 move;

    switch (sMoveRelearner->state)
    {
    case MENU_STATE_FADE_TO_BLACK:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        ShowBg(0);
        ShowBg(1);
        LoadMoveInfoUI();
        sMoveRelearner->state++;
        DrawTextBorderOnListAndMessage();
        PrintTeachWhichMoveToStrVar1(FALSE);
        MoveLearnerInitListMenu();
        sMoveRelearner->scheduleMoveInfoUpdate = TRUE;
        break;
    case MENU_STATE_WAIT_FOR_FADE:
        if (!gPaletteFade.active)
            sMoveRelearner->state = MENU_STATE_IDLE_BATTLE_MODE;
        break;
    case MENU_STATE_UNREACHABLE:
        sMoveRelearner->state++;
        break;
    case MENU_STATE_SETUP_BATTLE_MODE:
        PrintTeachWhichMoveToStrVar1(FALSE);
        sMoveRelearner->scheduleMoveInfoUpdate = TRUE;
        sMoveRelearner->state++;
        break;
    case MENU_STATE_IDLE_BATTLE_MODE:
        MoveRelearnerMenuHandleInput();
        break;
    case MENU_STATE_PRINT_TEACH_MOVE_PROMPT:
        MoveRelearnerCreateYesNoMenu();
        sMoveRelearner->state++;
        break;
    case MENU_STATE_TEACH_MOVE_CONFIRM:
        switch (YesNoMenuProcessInput())
        {
        case 0:
            if (GiveMoveToMon(&gPlayerParty[sMoveRelearner->partyMon], sMoveRelearner->movesToLearn[sMoveRelearner->selectedIndex]) != MON_HAS_MAX_MOVES)
            {
                MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerPkmnLearnedMove);
                gSpecialVar_0x8004 = TRUE;
                sMoveRelearner->state = MENU_STATE_PRINT_TEXT_THEN_FANFARE;
            }
            else
            {
                sMoveRelearner->state = MENU_STATE_PRINT_TRYING_TO_LEARN_PROMPT;
            }
            break;
        case 1:
        case MENU_B_PRESSED:
            sMoveRelearner->state = MENU_STATE_SETUP_BATTLE_MODE;
            break;
        }
        break;
    case MENU_STATE_PRINT_GIVE_UP_PROMPT:
        MoveRelearnerCreateYesNoMenu();
        sMoveRelearner->state++;
        break;
    case MENU_STATE_GIVE_UP_CONFIRM:
        switch (YesNoMenuProcessInput())
        {
        case 0:
            sMoveRelearner->state = MENU_STATE_SETUP_BATTLE_MODE;
            break;
        case 1:
        case MENU_B_PRESSED:
            gSpecialVar_0x8004 = FALSE;
            sMoveRelearner->state = MENU_STATE_FADE_AND_RETURN;
            break;
        }
        break;
    case MENU_STATE_PRINT_TRYING_TO_LEARN_PROMPT:
        MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerPkmnTryingToLearnMove);
        sMoveRelearner->state++;
        break;
    case MENU_STATE_WAIT_FOR_TRYING_TO_LEARN:
        MoveRelearnerCreateYesNoMenu();
        sMoveRelearner->state = MENU_STATE_CONFIRM_DELETE_OLD_MOVE;
        break;
    case MENU_STATE_CONFIRM_DELETE_OLD_MOVE:
        switch (YesNoMenuProcessInput())
        {
        case 0:
            MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerWhichMoveToForget);
            sMoveRelearner->state = MENU_STATE_PRINT_WHICH_MOVE_PROMPT;
            break;
        case 1:
        case MENU_B_PRESSED:
            sMoveRelearner->state = MENU_STATE_PRINT_STOP_TEACHING;
            break;
        }
        break;
    case MENU_STATE_PRINT_STOP_TEACHING:
        StringCopy(gStringVar2, GetMoveName(sMoveRelearner->movesToLearn[sMoveRelearner->selectedIndex]));
        MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerStopTryingToTeachMove);
        sMoveRelearner->state++;
        break;
    case MENU_STATE_WAIT_FOR_STOP_TEACHING:
        MoveRelearnerCreateYesNoMenu();
        sMoveRelearner->state = MENU_STATE_CONFIRM_STOP_TEACHING;
        break;
    case MENU_STATE_CONFIRM_STOP_TEACHING:
        switch (YesNoMenuProcessInput())
        {
        case 0:
            sMoveRelearner->state = MENU_STATE_PRINT_TRYING_TO_LEARN_PROMPT;
            break;
        case 1:
        case MENU_B_PRESSED:
            sMoveRelearner->state = MENU_STATE_CHOOSE_SETUP_STATE;
            break;
        }
        break;
    case MENU_STATE_CHOOSE_SETUP_STATE:
        sMoveRelearner->state = MENU_STATE_SETUP_BATTLE_MODE;
        break;
    case MENU_STATE_PRINT_WHICH_MOVE_PROMPT:
        sMoveRelearner->state = MENU_STATE_SHOW_MOVE_SUMMARY_SCREEN;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        break;
    case MENU_STATE_SHOW_MOVE_SUMMARY_SCREEN:
        if (!gPaletteFade.active)
        {
            ListMenuGetScrollAndRow(sMoveRelearner->moveListMenuTask, &sMoveRelearner->listMenuScrollPos, &sMoveRelearner->listMenuScrollRow);
            FreeAllWindowBuffers();
            ShowSelectMovePokemonSummaryScreen(gPlayerParty, sMoveRelearner->partyMon, gPlayerPartyCount - 1, CB2_InitLearnMoveReturnFromSelectMove_RG, sMoveRelearner->movesToLearn[sMoveRelearner->selectedIndex]);
            
            sMoveRelearner->state = MENU_STATE_FADE_FROM_SUMMARY_SCREEN;
        }
        break;
    case MENU_STATE_FADE_AND_RETURN:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        sMoveRelearner->state++;
        break;
    case MENU_STATE_RETURN_TO_FIELD:
        if (!gPaletteFade.active)
        {
            if (gInitialSummaryScreenCallback != NULL)
            {
                switch (gOriginSummaryScreenPage)
                {
                case PSS_PAGE_BATTLE_MOVES:
                    ShowPokemonSummaryScreen(SUMMARY_MODE_RELEARNER_BATTLE, gPlayerParty, sMoveRelearner->partyMon, gPlayerPartyCount - 1, gInitialSummaryScreenCallback);
                    break;
                case PSS_PAGE_CONTEST_MOVES:
                    ShowPokemonSummaryScreen(SUMMARY_MODE_RELEARNER_CONTEST, gPlayerParty, sMoveRelearner->partyMon, gPlayerPartyCount - 1, gInitialSummaryScreenCallback);
                    break;
                default:
                    ShowPokemonSummaryScreen(SUMMARY_MODE_NORMAL, gPlayerParty, sMoveRelearner->partyMon, gPlayerPartyCount - 1, gInitialSummaryScreenCallback);
                    break;
                }
                gOriginSummaryScreenPage = 0;
            }
            else
            {
                SetMainCallback2(CB2_ReturnToField);
            }
            FreeAllWindowBuffers();
            Free(sMoveRelearner);
        }
        break;
    case MENU_STATE_FADE_FROM_SUMMARY_SCREEN:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        ShowBg(0);
        ShowBg(1);
        sMoveRelearner->state++;
        LoadMoveInfoUI();
        DrawTextBorderOnListAndMessage();
        MoveLearnerInitListMenu();
        PrintTeachWhichMoveToStrVar1(TRUE);
        PrintMoveInfoHandleCancel_CopyToVram();
        break;
    case MENU_STATE_TRY_OVERWRITE_MOVE:
        if (!gPaletteFade.active)
        {
            if (sMoveRelearner->moveSlot == MAX_MON_MOVES)
            {
                sMoveRelearner->state = MENU_STATE_PRINT_STOP_TEACHING;
            }
            else
            {
                move = GetMonData(&gPlayerParty[sMoveRelearner->partyMon], MON_DATA_MOVE1 + sMoveRelearner->moveSlot);
                u8 originalPP = GetMonData(&gPlayerParty[sMoveRelearner->partyMon], MON_DATA_PP1 + sMoveRelearner->moveSlot);
                StringCopy(gStringVar3, GetMoveName(move));
                RemoveMonPPBonus(&gPlayerParty[sMoveRelearner->partyMon], sMoveRelearner->moveSlot);
                SetMonMoveSlot(&gPlayerParty[sMoveRelearner->partyMon], sMoveRelearner->movesToLearn[sMoveRelearner->selectedIndex], sMoveRelearner->moveSlot);
                u8 newPP = GetMonData(&gPlayerParty[sMoveRelearner->partyMon], MON_DATA_PP1 + sMoveRelearner->moveSlot);
                if (!P_SUMMARY_MOVE_RELEARNER_FULL_PP && gOriginSummaryScreenPage != 0 && originalPP < newPP)
                    SetMonData(&gPlayerParty[sMoveRelearner->partyMon], MON_DATA_PP1 + sMoveRelearner->moveSlot, &originalPP);
                StringCopy(gStringVar2, GetMoveName(sMoveRelearner->movesToLearn[sMoveRelearner->selectedIndex]));
                MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerAndPoof);
                sMoveRelearner->state = MENU_STATE_DOUBLE_FANFARE_FORGOT_MOVE;
                gSpecialVar_0x8004 = TRUE;
            }
        }
        break;
    case MENU_STATE_DOUBLE_FANFARE_FORGOT_MOVE:
        MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerPkmnForgotMoveAndLearnedNew);
        sMoveRelearner->state = MENU_STATE_PRINT_TEXT_THEN_FANFARE;
        PlayFanfare(MUS_LEVEL_UP);
        break;
    case MENU_STATE_PRINT_TEXT_THEN_FANFARE:
        PlayFanfare(MUS_LEVEL_UP);
        sMoveRelearner->state = MENU_STATE_WAIT_FOR_FANFARE;
        break;
    case MENU_STATE_WAIT_FOR_FANFARE:
        if (IsFanfareTaskInactive())
            sMoveRelearner->state = MENU_STATE_WAIT_FOR_A_BUTTON;
        break;
    case MENU_STATE_WAIT_FOR_A_BUTTON:
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            sMoveRelearner->state = MENU_STATE_FADE_AND_RETURN;
        }
        break;
    }
}

static void DrawTextBorderOnListAndMessage(void)
{
    int i;
    for (i = RELEARN_WINDOW_MOVE_LIST; i < RELEARN_WINDOW_COUNT; i++)
        DrawTextBorderOuter(i, 0x001, 14);
}


static const u8 sText_TeachWhichMoveToPkmn[] = _("Teach which move to\n{STR_VAR_1}?");
static const u8 sText_ViewLevelMovesForPkmn[] = _("Viewing level-up set for\n{STR_VAR_1}.");
static const u8 sText_ViewEggMovesForPkmn[] = _("Viewing egg moves for\n{STR_VAR_1}.");

static void PrintTeachWhichMoveToStrVar1(bool8 onInit)
{
    if (!onInit)
    {
        StringExpandPlaceholders(gStringVar4, sText_TeachWhichMoveToPkmn);
        PrintTextOnWindow(RELEARN_WINDOW_MESSAGE, gStringVar4, 0, 2, 0, RELEARN_COLOR_ALT);
        PutWindowTilemap(RELEARN_WINDOW_MESSAGE);
        CopyWindowToVram(RELEARN_WINDOW_MESSAGE, COPYWIN_FULL);
    }
}

static void InitMoveRelearnerStateVariables(void)
{
    int i;
    sMoveRelearner->state = 0;
    sMoveRelearner->numMenuChoices = 0;
    sMoveRelearner->numToShowAtOnce = 0;
    sMoveRelearner->scheduleMoveInfoUpdate = FALSE;
    for (i = 0; i < MAX_LEVEL_UP_MOVES; i++)
        sMoveRelearner->movesToLearn[i] = MOVE_NONE;
}

static void MoveRelearnerInitListMenuBuffersEtc(void)
{
    int i;
    s32 count;
    u8 nickname[POKEMON_NAME_LENGTH + 1];

    sMoveRelearner->numMenuChoices = GetMoveRelearnerMoves(&gPlayerParty[sMoveRelearner->partyMon], sMoveRelearner->movesToLearn);
    count = GetMoveRelearnerMoves(&gPlayerParty[sMoveRelearner->partyMon], sMoveRelearner->movesToLearn);
    GetMonData(&gPlayerParty[sMoveRelearner->partyMon], MON_DATA_NICKNAME, nickname);
    StringCopy_Nickname(gStringVar1, nickname);
    sMoveRelearner->numMenuChoices++;
    for (i = 0; i < count; i++)
    {
        sMoveRelearner->menuItems[i].name = GetMoveName(sMoveRelearner->movesToLearn[i]);
        sMoveRelearner->menuItems[i].id = i;
    }
    sMoveRelearner->menuItems[i].name = gText_Cancel;
    sMoveRelearner->menuItems[i].id = LIST_CANCEL;
    gMultiuseListMenuTemplate = sMoveRelearnerListMenuTemplate;
    gMultiuseListMenuTemplate.items = sMoveRelearner->menuItems;
    if (sMoveRelearner->numMenuChoices < 6)
        gMultiuseListMenuTemplate.maxShowed = sMoveRelearner->numMenuChoices;
    sMoveRelearner->numToShowAtOnce = gMultiuseListMenuTemplate.maxShowed;
    gMultiuseListMenuTemplate.totalItems = count + 1;
}

static void MoveRelearnerMenuHandleInput(void)
{
    ListMenu_ProcessInput(sMoveRelearner->moveListMenuTask);
    if (JOY_NEW(A_BUTTON))
    {
        if (sMoveRelearner->selectedIndex != LIST_CANCEL)
        {
            PlaySE(SE_SELECT);
            sMoveRelearner->state = MENU_STATE_PRINT_TEACH_MOVE_PROMPT;
            StringCopy(gStringVar2, GetMoveName(sMoveRelearner->movesToLearn[sMoveRelearner->selectedIndex]));
            MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerTeachMoveConfirm);
        }
        else if (sMoveRelearner->selectedIndex == LIST_CANCEL)
        {
            PlaySE(SE_SELECT);
            sMoveRelearner->state = MENU_STATE_PRINT_GIVE_UP_PROMPT;
            MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerGiveUp);
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        sMoveRelearner->state = MENU_STATE_PRINT_GIVE_UP_PROMPT;
        MoveRelearnerExpandAndPrintMessage(gText_MoveRelearnerGiveUp);
    }
}

static void MoveLearnerInitListMenu(void)
{
    sMoveRelearner->moveListMenuTask = ListMenuInit(&gMultiuseListMenuTemplate, sMoveRelearner->listMenuScrollPos, sMoveRelearner->listMenuScrollRow);
    CopyWindowToVram(RELEARN_WINDOW_MOVE_LIST, COPYWIN_MAP);
}

// credit to Vexx on PRET Discord
static void FormatTextByWidth(u8 *result, s32 maxWidth, u8 fontId, const u8 *str, s16 letterSpacing)
{
    u8 *end, *ptr, *curLine, *lastSpace;

    end = result;
    // copy string, replacing all space and line breaks with EOS
    while (*str != EOS)
    {
        if (*str == CHAR_SPACE || *str == CHAR_NEWLINE)
            *end = EOS;
        else
            *end = *str;

        end++;
        str++;
    }
    *end = EOS; // now end points to the true end of the string

    ptr = result;
    curLine = ptr;

    while (*ptr != EOS)
        ptr++;
    // now ptr is the first EOS char

    while (ptr != end)
    {
        // all the EOS chars (except *end) must be replaced by either ' ' or '\n'
        lastSpace = ptr++; // this points at the EOS

        // check that adding the next word this line still fits
        *lastSpace = CHAR_SPACE;
        if (GetStringWidth(fontId, curLine, letterSpacing) > maxWidth)
        {
            *lastSpace = CHAR_NEWLINE;

            curLine = ptr;
        }

        while (*ptr != EOS)
            ptr++;
        // now ptr is the next EOS char
    }
}

static void PrintMoveInfo(u16 move)
{
    u8 buffer[120];
    BlitMenuInfoIcon(RELEARN_WINDOW_MOVE_ICONS_3, GetMoveType(move) + 1, 1, 4);
    BlitMenuInfoIcon(RELEARN_WINDOW_MOVE_ICONS_3, GetMoveCategory(move) + MENU_INFO_ICON_PHYSICAL, 9, 19);

    if (GetMovePower(move) < 2)
    {
        PrintTextOnWindow(RELEARN_WINDOW_MOVE_STATS, gText_ThreeDashes, 1, 4, 0, RELEARN_COLOR_NORMAL);
    }
    else
    {
        ConvertIntToDecimalStringN(buffer, GetMovePower(move), STR_CONV_MODE_RIGHT_ALIGN, 3);
        PrintTextOnWindow(RELEARN_WINDOW_MOVE_STATS, buffer, 1, 4, 0, RELEARN_COLOR_NORMAL);
    }

    if (GetMoveAccuracy(move) == 0)
    {
        PrintTextOnWindow(RELEARN_WINDOW_MOVE_STATS, gText_ThreeDashes, 1, 18, 0, RELEARN_COLOR_NORMAL_SKIP);
    }
    else
    {
        ConvertIntToDecimalStringN(buffer, GetMoveAccuracy(move), STR_CONV_MODE_RIGHT_ALIGN, 3);
        PrintTextOnWindow(RELEARN_WINDOW_MOVE_STATS, buffer, 1, 18, 0, RELEARN_COLOR_NORMAL_SKIP);
    }
    ConvertIntToDecimalStringN(buffer, GetMovePP(move), STR_CONV_MODE_RIGHT_ALIGN, 3);
    PrintTextOnWindow(RELEARN_WINDOW_MOVE_STATS, buffer, 1, 32, 0, RELEARN_COLOR_NORMAL_SKIP);

    FormatTextByWidth(buffer, 116, FONT_SHORT, GetMoveDescription(move), 0);;
    PrintTextOnWindow(RELEARN_WINDOW_MOVE_DESCRIPTION, buffer, 1, 0, 0, RELEARN_COLOR_NORMAL);
}

static void LoadMoveInfoUI(void)
{
    BlitMenuInfoIcon(RELEARN_WINDOW_MOVE_ICONS_1, MENU_INFO_ICON_TYPE, 1, 4);
    BlitMenuInfoIcon(RELEARN_WINDOW_MOVE_ICONS_1, MENU_INFO_ICON_CATEGORY, 1, 19);
    BlitMenuInfoIcon(RELEARN_WINDOW_MOVE_ICONS_1, MENU_INFO_ICON_EFFECT, 1, 34);
    BlitMenuInfoIcon(RELEARN_WINDOW_MOVE_ICONS_2, MENU_INFO_ICON_POWER, 0, 4);
    BlitMenuInfoIcon(RELEARN_WINDOW_MOVE_ICONS_2, MENU_INFO_ICON_ACCURACY, 0, 19);
    BlitMenuInfoIcon(RELEARN_WINDOW_MOVE_ICONS_2, MENU_INFO_ICON_PP, 0, 34);
    PutWindowTilemap(RELEARN_WINDOW_MOVE_ICONS_1);
    PutWindowTilemap(RELEARN_WINDOW_MOVE_ICONS_2);
    PutWindowTilemap(RELEARN_WINDOW_MOVE_STATS);
    PutWindowTilemap(RELEARN_WINDOW_MOVE_DESCRIPTION);
    PutWindowTilemap(RELEARN_WINDOW_MOVE_ICONS_3);
    PutWindowTilemap(RELEARN_WINDOW_MESSAGE);
    CopyWindowToVram(RELEARN_WINDOW_MOVE_ICONS_1, COPYWIN_GFX);
    CopyWindowToVram(RELEARN_WINDOW_MOVE_ICONS_2, COPYWIN_GFX);
}

static void PrintMoveInfoHandleCancel_CopyToVram(void)
{
    int i;
    if (sMoveRelearner->selectedIndex != LIST_CANCEL)
    {
        PrintMoveInfo(sMoveRelearner->movesToLearn[sMoveRelearner->selectedIndex]);
    }
    else
    {
        for (i = RELEARN_WINDOW_MOVE_ICONS_3; i < RELEARN_WINDOW_MOVE_LIST; i++)
        {
            FillWindowPixelBuffer(i, PIXEL_FILL(0));
            CopyWindowToVram(i, COPYWIN_GFX);
        }
    }
    CopyWindowToVram(RELEARN_WINDOW_MOVE_STATS, COPYWIN_GFX);
    CopyWindowToVram(RELEARN_WINDOW_MOVE_ICONS_3, COPYWIN_GFX);
    CopyWindowToVram(RELEARN_WINDOW_MOVE_ICONS_3, COPYWIN_GFX);
    CopyWindowToVram(RELEARN_WINDOW_MOVE_DESCRIPTION, COPYWIN_GFX);
    CopyWindowToVram(RELEARN_WINDOW_MESSAGE, COPYWIN_FULL);
}

static void MoveRelearnerMenu_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    if (!onInit)
    {
        PlaySE(SE_SELECT);
        sMoveRelearner->scheduleMoveInfoUpdate = TRUE;
        sMoveRelearner->selectedIndex = itemIndex;
    }
}

static s8 YesNoMenuProcessInput(void)
{
    s8 input = Menu_ProcessInputNoWrapClearOnChoose();
    if (input != MENU_NOTHING_CHOSEN)
    {
        PutWindowTilemap(RELEARN_WINDOW_MOVE_LIST);
        CopyWindowToVram(RELEARN_WINDOW_MOVE_LIST, COPYWIN_MAP);
    }
    return input;
}

static void PrintTextOnWindow(u8 windowId, const u8 *str, u8 x, u8 y, s32 speed, s32 colorIdx)
{
    s32 letterSpacing = 1;
    s32 lineSpacing = 1;
    if (colorIdx == RELEARN_COLOR_NORMAL || colorIdx == RELEARN_COLOR_NORMAL_SKIP)
    {
        letterSpacing = 0;
        lineSpacing = 0;
    }
    if (colorIdx != RELEARN_COLOR_NORMAL_SKIP)
        FillWindowPixelBuffer(windowId, PIXEL_FILL(sRelearnTextColors[colorIdx][0]));
    AddTextPrinterParameterized4(windowId, FONT_SHORT, x, y, letterSpacing, lineSpacing, sRelearnTextColors[colorIdx], speed, str);
}
