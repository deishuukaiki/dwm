/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const int statmonval = 0;
static const unsigned int snap      = 10;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 1;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "SauceCodePro Nerd Font:size=10" };
static const char dmenufont[]       = "SauceCodePro Nerd Font:size=10";
static const char col_bg[] 	= "#1f1f28";
static const char col_fg[]	= "#dcd7ba";
static const char col_sel[]      = "#664d77";
static const char col_border[]	    = "#755b93";
static const char col_uborder[]	    = "#969896";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg, col_bg, col_uborder },
	[SchemeSel]  = { col_fg, col_sel,  col_border  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isterminal   noswallow   monitor */
	{"pulsemixer",  NULL,   	NULL,   	0,          	1,          0,	        0,      	-1},
	{"qalc",        NULL,   	NULL,	    0,          	1,          0,	        0,          -1},

	{"qjackctl",    NULL,   	NULL,	    0,          	1,          0,      	0,      	-1},
	{NULL,          "qjackctl", NULL,	    0,          	1,          0,	        0,      	-1},

    /* steam */
	{"Steam",       NULL,   	NULL,	    0,          	1,          0,          0,          -1},
	{"steam_app_1182480",NULL,  NULL,       0,          	1,          0,          0,          -1},
	{NULL,      	NULL,	"Steam",        0,          	0,          0,          0,          -1},

    /* origin */
	{"Origin",      NULL,       NULL,       0,          	1,          0,          0,          -1},
	{"origin.exe",  NULL,       NULL,       0,          	1,          0,          0,          -1},
	{NULL,          "Origin",   NULL,       0,          	1,          0,          0,          -1},
	{NULL,          "origin.exe",NULL,      0,          	1,          0,          0,          -1},
	{NULL,          NULL,       "Origin",   0,          	1,          0,          0,          -1},
	{NULL,          NULL,       "origin.exe",0,          	1,          0,          0,          -1},


	/*swallow*/
	{"st",          NULL,       NULL,       0,              0,          1,          0,          -1},
	{"kitty",       NULL,       NULL,       0,              0,          1,          0,          -1},
	{NULL,          NULL,       "EventTester",0,            0,          0,          1,          -1},/*xev*/
    {"libreoffice", NULL,      NULL,       0,              0,          0,          1,          -1},
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "D[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

static const MonitorRule monrules[] = {
	/* monitor  tag  layout  mfact  nmaster  showbar  topbar */
	{  0,       -1,  0,      -1,    -1,      -1,      -1     }, // use a different layout for the second monitor
	{  1,       -1,  5,      -1,    -1,      -1,      -1     }, // use a different layout for the second monitor
	{  2,       -1,  0,      -1,    -1,      -1,      -1     }, // use a different layout for the second monitor
	{  -1,      -1,  0,      -1,    -1,      -1,      -1     }, // default
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTMOD Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, 


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#include <X11/XF86keysym.h>

/* commands */
static char dmenumon[2] = "1"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_history", "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_sel, "-sf", col_fg, NULL };
static const char *dmenutodocmd[] = { "dmenu_todo", "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_sel, "-sf", col_fg, NULL };
static const char *dmenuxmms2cmd[] = { "dmenu_xmms2", "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_sel, "-sf", col_fg, NULL };
static const char *dmenupassmenu2cmd[] = { "dmenu_passmenu2", "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_sel, "-sf", col_fg, NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "kitty", "-o", "initial_window_height=480", "-o", "initial_window_width=640", "--class", scratchpadname, "--title", scratchpadname, NULL };

static const char *termcmd[]  = { "kitty", NULL };
static const char *browsercmd[] = { "qutebrowser", NULL };
static const char *pulsemixercmd[] = { "kitty", "-o", "initial_window_height=480", "-o", "initial_window_width=640", "--class", "pulsemixer", "--title", "pulsemixer", "pulsemixer", NULL };
static const char *qalccmd[] = { "kitty", "-o", "initial_window_height=480", "-o", "initial_window_width=640", "--class", "qalc", "--title", "qalc", "qalc" };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */

	/* brightness */
	{ 0,				XF86XK_MonBrightnessUp,		spawn,	SHCMD("xbacklight -inc 10")},
	{ 0,				XF86XK_MonBrightnessDown,	spawn,	SHCMD("xbacklight -dec 10")},

	/* volume */
	
	{ 0,				XF86XK_AudioRaiseVolume,	spawn,	SHCMD("pulsemixer --change-volume +2; pkill -RTMIN+1 dwmblocks") },
	{ 0,				XF86XK_AudioLowerVolume,	spawn,	SHCMD("pulsemixer --change-volume -2; pkill -RTMIN+1 dwmblocks") },
	{ 0,				XF86XK_AudioMute,		spawn,	SHCMD("pulsemixer --toggle-mute; pkill -RTMIN+1 dwmblocks") },

	/* run programs */
  
	{ MODKEY,                       XK_a,      spawn,          {.v = dmenutodocmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = dmenuxmms2cmd } },
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = dmenupassmenu2cmd } },
	{ MODKEY,			            XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,		        XK_Return, spawn,          {.v = browsercmd } },
	{ MODKEY,			            XK_u,	   spawn,	   {.v = pulsemixercmd } },
	{ MODKEY,			            XK_i,	   spawn,	   {.v = qalccmd } },
	{ MODKEY,                       XK_o,  	   togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

	/* screenshot */

	{ 0,				            XK_Print,  spawn,	   SHCMD("exec flameshot gui") },
	{ MODKEY,			            XK_Print,  spawn,	   SHCMD("exec flameshot screen -p ~/downloads/ -c") },
	{ MODKEY|ShiftMask,		        XK_Print,  spawn,	   SHCMD("exec flameshot full -p ~/downloads/ -c") },
	
	/* windows */

	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },

	/* tags */

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	/* TAGKEYS(                        XK_6,                      5) */
	/* TAGKEYS(                        XK_7,                      6) */
	/* TAGKEYS(                        XK_8,                      7) */
	/* TAGKEYS(                        XK_9,                      8) */

	/* cfacts, mfacts, gaps, bar, layouts */

	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_BackSpace,setmfact,     {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_BackSpace,setcfact,     {.f =  0.00} },
	{ MODKEY,                       XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY,                       XK_period, cyclelayout,    {.i = +1 } },

    /* floating moveresize */
	{ MODKEY|ControlMask,                       XK_h,   moveresize,     {.v = "-50x 0y 0w 0h" } },
  	{ MODKEY|ControlMask,                       XK_j,   moveresize,     {.v = "0x 50y 0w 0h" } },
	{ MODKEY|ControlMask,                       XK_k,     moveresize,     {.v = "0x -50y 0w 0h" } },
	{ MODKEY|ControlMask,                       XK_l,  moveresize,     {.v = "50x 0y 0w 0h" } },
	{ MODKEY|ControlMask|ShiftMask,             XK_h,   moveresize,     {.v = "0x 0y -50w 0h" } },
	{ MODKEY|ControlMask|ShiftMask,             XK_j,   moveresize,     {.v = "0x 0y 0w 50h" } },
	{ MODKEY|ControlMask|ShiftMask,             XK_k,     moveresize,     {.v = "0x 0y 0w -50h" } },
	{ MODKEY|ControlMask|ShiftMask,             XK_l,  moveresize,     {.v = "0x 0y 50w 0h" } },


	/* stack navigation */

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_space,  zoom,           {0} },

	/* monitor navigation */
	{ MODKEY,                       XK_q,      focusnthmon,     {.i = 0 } },
	{ MODKEY,                       XK_w,      focusnthmon,     {.i = 1 } },
	{ MODKEY,                       XK_e,      focusnthmon,     {.i = 2 } },
	{ MODKEY,                       XK_r,      focusnthmon,     {.i = 3 } },
	{ MODKEY|ShiftMask,             XK_q,      tagnthmon,       {.i = 0 } },
	{ MODKEY|ShiftMask,             XK_w,      tagnthmon,       {.i = 1 } },
	{ MODKEY|ShiftMask,             XK_e,      tagnthmon,       {.i = 2 } },
	{ MODKEY|ShiftMask,             XK_r,      tagnthmon,       {.i = 3 } },

	/* quit and restart */

	{ MODKEY|ShiftMask,             XK_End,      quit,           {0} },
	{ MODKEY|ControlMask, 		    XK_End,      quit,           {1} }, 
	{ MODKEY,			            XK_End,	     spawn,	     SHCMD("slock") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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
