TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += src

# Ease access to these files from Qt Creator
OTHER_FILES += \
    .gitignore \
    .qmake.conf \
    README.md
