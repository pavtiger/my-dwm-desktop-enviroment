/*
 * Copy me if you can.
 * by 20h
 */

#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <X11/Xlib.h>

char *tzutc = "UTC";
char *tzmoscow = "Europe/Moscow";

static Display *dpy;

char *
smprintf(char *fmt, ...)
{
    va_list fmtargs;
    char *ret;
    int len;

    va_start(fmtargs, fmt);
    len = vsnprintf(NULL, 0, fmt, fmtargs);
    va_end(fmtargs);

    ret = malloc(++len);
    if (ret == NULL)
    {
        perror("malloc");
        exit(1);
    }

    va_start(fmtargs, fmt);
    vsnprintf(ret, len, fmt, fmtargs);
    va_end(fmtargs);

    return ret;
}

void settz(char *tzname)
{
    setenv("TZ", tzname, 1);
}

char *
mktimes(char *fmt, char *tzname)
{
    char buf[129];
    time_t tim;
    struct tm *timtm;

    settz(tzname);
    tim = time(NULL);
    timtm = localtime(&tim);
    if (timtm == NULL)
        return smprintf("");

    if (!strftime(buf, sizeof(buf) - 1, fmt, timtm))
    {
        fprintf(stderr, "strftime == 0\n");
        return smprintf("");
    }

    return smprintf("%s", buf);
}

void setstatus(char *str)
{
    XStoreName(dpy, DefaultRootWindow(dpy), str);
    XSync(dpy, False);
}

char *
loadavg(void)
{
    double avgs[3];

    if (getloadavg(avgs, 3) < 0)
        return smprintf("");

    return smprintf("%.2f %.2f %.2f", avgs[0], avgs[1], avgs[2]);
}

char *
readfile(char *base, char *file)
{
    char *path, line[513];
    FILE *fd;

    memset(line, 0, sizeof(line));

    path = smprintf("%s/%s", base, file);
    fd = fopen(path, "r");
    free(path);
    if (fd == NULL)
        return NULL;

    if (fgets(line, sizeof(line) - 1, fd) == NULL)
        return NULL;
    fclose(fd);

    return smprintf("%s", line);
}

char *
getbattery(char *base)
{
    char *co, status;
    int descap, remcap;

    descap = -1;
    remcap = -1;

    co = readfile(base, "present");
    if (co == NULL)
        return smprintf("");
    if (co[0] != '1')
    {
        free(co);
        return smprintf("not present");
    }
    free(co);

    co = readfile(base, "charge_full_design");
    if (co == NULL)
    {
        co = readfile(base, "energy_full_design");
        if (co == NULL)
            return smprintf("");
    }
    sscanf(co, "%d", &descap);
    free(co);

    co = readfile(base, "charge_now");
    if (co == NULL)
    {
        co = readfile(base, "energy_now");
        if (co == NULL)
            return smprintf("");
    }
    sscanf(co, "%d", &remcap);
    free(co);

    co = readfile(base, "status");
    if (!strncmp(co, "Discharging", 11))
    {
        status = '-';
    }
    else if (!strncmp(co, "Charging", 8))
    {
        status = '+';
    }
    else
    {
        status = '?';
    }

    if (remcap < 0 || descap < 0)
        return smprintf("invalid");

    return smprintf("%.0f%%%c", ((float)remcap / (float)descap) * 100, status);
}

char *
gettemperature(char *base, char *sensor)
{
    char *co;

    co = readfile(base, sensor);
    if (co == NULL)
        return smprintf("");
    return smprintf("%02.0f°C", atof(co) / 1000);
}

char *
get_volume()
{
    FILE *fp;
    char buf[64];
    fp = popen("getvolume", "r");
    if (fp == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    if (fgets(buf, sizeof(buf), fp) != NULL)
    {
        pclose(fp);
        int l = strlen(buf);
        if (buf[l - 1] == '\n') {
            buf[l - 1] = '\0';
        }
        return smprintf("%s", buf);
    }
    pclose(fp);
    return smprintf("-1");
}

char *
get_lang()
{
    FILE *fp;
    char buf[16];
    fp = popen("xkblayout-state print %n", "r");
    if (fp == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    if (fgets(buf, sizeof(buf), fp) != NULL)
    {
        pclose(fp);
        return smprintf("%s", buf);
    }
    pclose(fp);
    return smprintf("-1");
}

char *
get_cpu()
{
    FILE *fp;
    char buf[6];
    fp = popen("getcpuload", "r");
    if (fp == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    if (fgets(buf, sizeof(buf), fp) != NULL)
    {
        pclose(fp);
        return smprintf("%04.1f", atof(buf));
    }
    pclose(fp);
    return smprintf("-1");
}

char *
get_player_status()
{
    FILE *fp;
    char buf[128];
    fp = popen("getplayerstatus", "r");
    if (fp == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    if (fgets(buf, sizeof(buf), fp) != NULL)
    {
        pclose(fp);
        int l = strlen(buf);
        if (buf[l - 1] == '\n') {
            buf[l - 1] = '\0';
        }
        return smprintf("%s", buf);
    }
    pclose(fp);
    return smprintf("No player running");
}

char *
get_song_name()
{
    FILE *fp;
    char buf[128];
    // fp = popen("getmedianame", "r");
    fp = popen("playerctl --player playerctld  metadata --format \"{{ artist }} - {{ title }}\"", "r");
    if (fp == NULL)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    if (fgets(buf, sizeof(buf), fp) != NULL)
    {
        pclose(fp);
        int l = strlen(buf);
        if (buf[l - 1] == '\n') {
            buf[l - 1] = '\0';
        }
        return smprintf("%s", buf);
    }
    pclose(fp);
    return smprintf("No Media is playing");
}

char *add_to_string(char *dest, char *src)
{
    char *dest_cp;
    dest_cp = dest;
    dest = malloc(sizeof(char) * strlen(dest) + strlen(src) + 1);
    strcpy(dest, dest_cp);
    strcat(dest, src);
    free(dest_cp);
    return dest;
}

int main(void)
{
    char *status;

    char *avgs;
    char *bat;
    char *tmmsk;

    char *player;
    char *media;

    char *vol;
    char *lang;
    char *load;

    if (!(dpy = XOpenDisplay(NULL)))
    {
        fprintf(stderr, "dwmstatus: cannot open display.\n");
        return 1;
    }

    for (;; usleep(100 * 1000))
    {
        avgs = loadavg();
        bat = getbattery("/sys/class/power_supply/BAT0");
        tmmsk = mktimes("%H:%M:%S %d/%b/%Y", tzmoscow);

        player = get_player_status();
        media = get_song_name();
        vol = get_volume();
        lang = get_lang();
        load = get_cpu();

        // init status
        status = malloc(1);
        status[0] = '\0';

        // media name
        status = add_to_string(status, "^c#FFE32E^");
	    char *media_info = smprintf("  %s %s ", media, player);
        status = add_to_string(status, media_info);
        free(media_info);

        // volume
        status = add_to_string(status, "^c#548EEB^");
        char *volume_info = smprintf(" %s ", vol);
        status = add_to_string(status, volume_info);
        free(volume_info);

        // load
        status = add_to_string(status, "^c#BE33FF^");
        char *load_info = smprintf("  %s%% ", load);
        status = add_to_string(status, load_info);
        free(load_info);

        // battery
        status = add_to_string(status, "\x08");
        char *battery_info = smprintf("  %s ", bat);
        status = add_to_string(status, battery_info);
        free(battery_info);

        // time
        status = add_to_string(status, "^c#50B38D^");
        char *time_info = smprintf("  %s ", tmmsk);
        status = add_to_string(status, time_info);
        free(time_info);

        // layout
        status = add_to_string(status, "^c#F24949^");
        char *lang_info = smprintf("  %s ", lang);
        status = add_to_string(status, lang_info);
        free(lang_info);

        // end status
        status = add_to_string(status, "^c#EEEEEE^");

        // status = smprintf("  \x06  \x07  \x08  \x09  \x01",
        //                     vol,
        //                     load,
        //                     bat,
        //                     tmmsk,
        //                     lang
        //                 );

        setstatus(status);

        free(avgs);
        free(bat);
        free(tmmsk);
        free(status);
        free(vol);
        free(lang);
        free(load);
    }

    XCloseDisplay(dpy);

    return 0;
}
