META
{
  Parser = Menu;
  After = zUnionMenu.d;
  Namespace = FriedMeat;
};

// Namespace = FriedMeat
// Important !!!
// Supporting Unicode symbols.
// File encoding: UTF-8 (without BOM).

// ------ Constants ------
const int Start_PY  = 1400;
const int Title_PY  = 450;
const int Menu_DY   = 550;
// Text
const int Text_PX   = 400;  // Position at left side
const int Text_SX   = 8000; // Size X
const int Text_SY   = 750;  // Size Y
const int Text_DY   = 120;  // Delta alignment
// Choice
const int Choice_PX = 6400; // Position at left side
const int Choice_SX = 1500; // Size X
const int Choice_SY = 350;  // Size Y
const int Choice_DY = 120;  // Delta alignment

const string MenuBackPic   = "UnionMenu_BackPic.tga";
const string ItemBackPic   = "";
const string ChoiceBackPic = "MENU_CHOICE_BACK.TGA";
const string FontSmall     = "font_old_10_white.tga";
const string FontBig       = "font_old_20_white.tga";

var int CurrentMenuItem_PY;

// ------ Prototypes ------
func void InitializeBackPicturesAndFonts()
{
  MenuBackPic   = MENU_BACK_PIC;
  ItemBackPic   = MENU_ITEM_BACK_PIC;
  ChoiceBackPic = MENU_CHOICE_BACK_PIC;
  FontSmall     = MENU_FONT_SMALL;
  FontBig       = MENU_FONT_DEFAULT;
};

prototype C_EMPTY_MENU_DEF(C_MENU)
{
  InitializeBackPicturesAndFonts();
  C_MENU_DEF();
  backpic    = MenuBackPic;
  items[0]   = "";
  items[100] = "Union_menuitem_back";
  flags      = flags | MENU_SHOW_INFO;
};

instance C_MENU_ITEM_TEXT_BASE(C_MENU_ITEM_DEF)
{
  backpic        = ItemBackPic;
  posx           = Text_PX;
  posy           = Start_PY;
  dimx           = Text_SX;
  dimy           = Text_SY;
  flags          = flags | IT_EFFECTS_NEXT;
  onselaction[0] = SEL_ACTION_UNDEF;
};

instance C_MENUITEM_CHOICE_BASE(C_MENU_ITEM_DEF)
{
  backpic  = ChoiceBackPic;
  type     = MENU_ITEM_CHOICEBOX;
  fontname = FontSmall;
  posx     = Choice_PX;
  posy     = Start_PY + Choice_DY;
  dimx     = Choice_SX;
  dimy     = Choice_SY;
  flags    = flags & ~IT_SELECTABLE;
  flags    = flags | IT_TXT_CENTER;
};

instance MenuItem_Opt_Headline(C_MENU_ITEM_DEF)
{
  type    = MENU_ITEM_TEXT;
  posx    = 0;
  posy    = Title_PY;
  dimx    = 8100;
  flags   = flags & ~IT_SELECTABLE;
  flags   = flags | IT_TXT_CENTER;
  text[0] = Str_GetLocalizedString(
    "FriedMeat НАСТРОЙКИ",
    "FriedMeat SETTINGS",
    "FriedMeat EINSTELLUNGEN",
    "USTAWIENIA FriedMeat"
  );
};

func int Act_OpenWebLink()
{
  Open_Link("https://github.com/Raster96/FriedMeat-Union");
  return 0;
};

instance MenuItem_Opt_Open_Link(C_MENU_ITEM_DEF)
{
  C_MENU_ITEM_TEXT_BASE();
  posy += MENU_DY * 8;

  posx             = 64;
  onselaction[0]   = SEL_ACTION_UNDEF;
  oneventaction[1] = Act_OpenWebLink;
  flags            = flags | IT_TXT_CENTER;
  text[0]          = Str_GetLocalizedString(
    "Открыть страницу проекта",
    "Open project page",
    "Projektseite öffnen",
    "Otwórz stronę projektu"
  );

  text[1]          = "https://github.com/Raster96/FriedMeat-Union";
};

// ------ Menu ------
instance MenuItem_Union_Auto_FriedMeat(C_MENU_ITEM_UNION_DEF)
{
  text[0]          = "FriedMeat";
  text[1] = Str_GetLocalizedString(
    "Настройте параметры FriedMeat",
    "Configure FriedMeat settings",
    "FriedMeat-Einstellungen konfigurieren",
    "Zmień ustawienia FriedMeat"
  );
  onselaction[0]   = SEL_ACTION_STARTMENU;
  onselaction_s[0] = "FriedMeat:Menu_Opt_FriedMeat";
};

instance Menu_Opt_FriedMeat(C_EMPTY_MENU_DEF)
{
  Menu_SearchItems("FriedMeat:MENUITEM_OPT_FriedMeat_*");
};

instance MenuItem_Opt_FriedMeat_Headline(C_MENU_ITEM)
{
  MenuItem_Opt_Headline();
};

// ------ Enabled ------
instance MenuItem_Opt_FriedMeat_Enabled(C_MENU_ITEM)
{
  CurrentMenuItem_PY = 1;
  C_MENU_ITEM_TEXT_BASE();
  fontname = FontSmall;
  posy += Menu_DY * CurrentMenuItem_PY + Text_DY;
  
  text[0] = "Enabled";
  text[1] = Str_GetLocalizedString(
    "Включить или отключить весь плагин",
    "Enable or disable the entire plugin",
    "Das gesamte Plugin aktivieren oder deaktivieren",
    "Włącz lub wyłącz cały plugin"
  );
};

instance MenuItem_Opt_FriedMeat_Enabled_Choice(C_MENU_ITEM_DEF)
{
  C_MENUITEM_CHOICE_BASE();
  posy += Menu_DY * CurrentMenuItem_PY;
  
  onchgsetoption        = "Enabled";
  onchgsetoptionsection = "FRIED_MEAT";
  text[0]               = Str_GetLocalizedString(
    "Выкл.|Вкл.",
    "Off|On",
    "Aus|Ein",
    "Wył.|Wł."
  );
};

// ------ Open Project Page ------
instance MenuItem_Opt_FriedMeat_Open_Link(C_MENU_ITEM)
{
  MenuItem_Opt_Open_Link();
};