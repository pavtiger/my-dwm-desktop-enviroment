# layouts (set Engish(US) and Russian keyboard layouts)
setxkbmap us,ru
# switch layouts by pressing "Super" and "Space" keys
setxkbmap -option 'grp:win_space_toggle'

# playerctd daemon
playerctld daemon

# statusbar
dwmstatus 2>&1 >/dev/null &

# set screen resolution as you want
xrandr --output DP-0 --mode 1920x1080 --rate 75 --primary --left-of HDMI-0
xrandr --output HDMI-0 --mode 1680x1050 --rate 60 --rigth-of DP-1

# disable screen sleep
xset s off -dpms

# set wallpapers
wallpapergen

# compositor
picom &