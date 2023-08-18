# layouts (set Engish(US) and Russian keyboard layouts)
setxkbmap us,ru
# switch layouts by pressing Alt + Shift
setxkbmap -option 'grp:alt_shift_toggle'

# playerctd daemon
playerctld daemon

# statusbar
dwmstatus 2>&1 >/dev/null &

# set screen resolution as you want
xrandr --output eDP-1 --mode 1920x1080 --primary

# disable screen sleep
xset s off -dpms

# set wallpapers
# wallpapergen

# wg-quick up amsterdam

# compositor
compton --backend glx -vsync opengl-swc -b &

