HEADERS += \
    $$files($$PWD/*.h) \
    $$files($$PWD/widgets/*h)

SOURCES += \
    $$files($$PWD/*.cpp) \
    $$files($$PWD/widget/*cpp) 

FORMS += \
    $$files($$PWD/forms/*.ui)
