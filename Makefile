all: wuav

wuav: wuav.vala
	valac wuav.vala --pkg=gio-2.0 --pkg=libsoup-2.4

.PHONY clean:
	rm -f wuav
