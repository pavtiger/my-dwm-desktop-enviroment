/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 0;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 1;     /* 0 means bottom bar */
static const char *fonts[]          = { "agave Nerd Font:size=10", "Cascadia Code:size=10"};
static const char dmenufont[]       = "Caskaydia Cove Nerd Font:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_breeze[]      = "#055749";
static const char col_nordic_gray[] = "#2a2f3a";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_nordic_gray,  col_nordic_gray  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Navigator",			NULL,       NULL,       1,       0,           0 },
	{ "firefox",			NULL,       NULL,       1,       0,           0 },
    { "cool-retro-term",	NULL,       NULL,       2,       0,           0 },

    { "jetbrains-clion",	NULL,       NULL,       4,       0,           0 },
    { "jetbrains-pycharm",	NULL,       NULL,       4,       0,           0 },
    { "jetbrains-webstorm",	NULL,       NULL,       4,       0,           0 },
    { "jetbrains-toolbox",	NULL,       NULL,       4,       0,           0 },
    { "jetbrains-client",	NULL,       NULL,       4,       0,           0 },

    { "Nemo",				NULL,       NULL,       8,       0,           0 },
    { "TelegramDesktop",	NULL,       NULL,       16,      0,           0 },

    { "broken",				NULL,       NULL,       32,      0,           0 },
    { "Spotify",			NULL,       NULL,       32,      0,           0 },

    { "zoom",				NULL,       NULL,       64,      0,           0 },
    { "mpv",				NULL,       NULL,       128,     0,           0 },
    { "Popcorn-Time",		NULL,       NULL,       128,     0,           0 },

    { "Blueman-manager",	NULL,       NULL,       256,     0,           0 },
    { "Pavucontrol",		NULL,       NULL,       256,     0,           0 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_breeze, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "cool-retro-term", NULL };
static const char *filescmd[] = { "thunar", NULL };

static const char *print_screen[] = { "scrot", "/home/pavtiger/Pictures/%Y-%m-%d-%H:%M.png", "--select", "--freeze", "--silent", "-l", "style=dash,width=3", NULL };
static const char *print_whole_screen[] = { "scrot", "/home/pavtiger/Pictures/%Y-%m-%d-%H:%M.png", "--silent", NULL };

static const char *swap_wallpaper[] = { "wallpapergen", NULL };
static const char *wallpapergen_menu[] = { "wallpapergen", "--menu", NULL };

static const char *up_vol[]   = { "sinkvolup",     NULL };
static const char *down_vol[] = { "sinkvoldown",   NULL };
static const char *mute_vol[] = { "sinkmute",          NULL };

static const char *previous[] = { "/usr/bin/playerctl", "previous", NULL};
static const char *play_pause[] = { "/usr/bin/playerctl", "play-pause", NULL};
static const char *next[] = { "/usr/bin/playerctl", "next", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
    // spawn programs
	{ MODKEY,               XK_p,                       spawn,          { .v = dmenucmd } },
    { MODKEY,               XK_Return,                  spawn,          { .v = termcmd } },
//  { MODKEY,               XK_e,                       spawn,          { .v = filescmd } },
	{ MODKEY,               XK_g,                       spawn,          { .v = swap_wallpaper } },  // generate new background images
    { MODKEY,               XK_w,                       spawn,          { .v = wallpapergen_menu } },  // main wallpapergen menu window

    { MODKEY,               XK_Print,                   spawn,          { .v = print_screen } },  // print screen
    { MODKEY,               XK_z,                       spawn,          { .v = print_whole_screen } },  // print whole screen

    // Close program
	{ MODKEY|ShiftMask,     XK_c,                       killclient,     {0} },

    // Exit dwm
	{ MODKEY|ShiftMask,     XK_q,                       quitprompt,           {0} },

    // Audio controls
	{ 0,                    XF86XK_AudioLowerVolume,    spawn,          {.v = down_vol } },
	{ 0,                    XF86XK_AudioMute,           spawn,          {.v = mute_vol } },
	{ 0,                    XF86XK_AudioRaiseVolume,    spawn,          {.v = up_vol   } },

    // Media controls
	{ 0,                    XF86XK_AudioPlay,           spawn,          {.v = play_pause } },
	{ 0,                    XF86XK_AudioPrev,	        spawn,          {.v = previous } },
	{ 0,                    XF86XK_AudioNext,           spawn,          {.v = next   } },

    // Monitor switching
	{ MODKEY,               XK_comma,                   focusmon,       {.i = -1 } },
	{ MODKEY,               XK_period,                  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,     XK_comma,                   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,     XK_period,                  tagmon,         {.i = +1 } },

    // Toggle bar
	{ MODKEY,               XK_b,                       togglebar,      {0} },

    // Master & stack navigation
	{ MODKEY,               XK_j,                       focusstack,     {.i = +1 } },
	{ MODKEY,               XK_k,                       focusstack,     {.i = -1 } },
	{ MODKEY,               XK_e,                       incnmaster,     {.i = +1 } },
	{ MODKEY,               XK_d,                       incnmaster,     {.i = -1 } },
	{ MODKEY,               XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY,               XK_l,                       setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,     XK_Return,                  zoom,           {0} },

    // Layouts
	{ MODKEY,               XK_t,                       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,               XK_f,                       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,               XK_m,                       setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,     XK_space,                   togglefloating, {0} },

    // Toggle last two tags
	{ MODKEY,               XK_Tab,                     view,           {0} },

    // Show all tags
	{ MODKEY,               XK_0,                       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,     XK_0,                       tag,            {.ui = ~0 } },

    // Tag switching
    TAGKEYS(                XK_1,                                   0)
    TAGKEYS(                XK_2,                                   1)
    TAGKEYS(                XK_3,                                   2)
    TAGKEYS(                XK_4,                                   3)
    TAGKEYS(                XK_5,                                   4)
    TAGKEYS(                XK_6,                                   5)
    TAGKEYS(                XK_7,                                   6)
    TAGKEYS(                XK_9,                                   8)
    TAGKEYS(                XK_8,                                   7)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

