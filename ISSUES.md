todo:
- create multiline buffer viewer/editor with scrollbar, linewrap, no-linewrap, live updating
- finish full table-view, with row/column headers and scrollbar
- checkbox widget. Will derive off button. Will not include check label.
- spinner widget
- tabs container
- scrollbar dragging
- sash-layout w/ dragging(sash display may be hline or vline, independent of sash control orientation. Will support ratios, initial sizing, respect max/min size of child widgets.)
- toolbox container [m|m|m|expanded window|m]  (m, minimized)
- add auto scroll container when contents are larger than window
- tab focus control
- group box (box with label)
- hline/vline, probably property of spacer
- button shadows
- color schemes

done:
- create single shot timing mechanism/events to allow things like button flashing to hook into select loop instead of hard sleep.

bugs:
- Single character width button not showing label.
- ListView last column not full sized.

bugs-fixed:
- Layout must respect size maximum. (a25a3d5dee112995884deeeaf8e2943489c18c7b)
- Layout off by one. (a25a3d5dee112995884deeeaf8e2943489c18c7b)

stretch goal:
- graphics rendering widget
    ~ line graphics rendering
    ~ unicode/braille rendering
    ~ half/whole block rendering: http://www.unicode.org/L2/Historical/FrankDaCruz-Archive-UCS-Terminal-Info/ucsterminal.txt
