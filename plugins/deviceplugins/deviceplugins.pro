TEMPLATE = subdirs

SUBDIRS += wakeonlan    \
    openweathermap      \
    datetime            \
    commandlauncher     \


!ubuntu {
SUBDIRS += elro         \
    intertechno         \
    wifidetector        \
    conrad              \
    mock                \
    lircd               \
    mailnotification    \
    philipshue          \
    eq-3                \
    wemo                \
    lgsmarttv           \
    genericelements     \
    unitec              \
    leynew              \

boblight {
    SUBDIRS += boblight
}
}
