
OBJS := ../am_map.o ../d_client.o ../d_deh.o ../d_items.o ../d_main.o ../doomdef.o \
	../doomstat.o ../dstrings.o ../f_finale.o ../f_wipe.o ../g_game.o \
	../gl_main.o ../gl_texture.o ../hu_lib.o ../hu_stuff.o ../info.o ../lprintf.o \
	../m_argv.o ../m_bbox.o ../m_cheat.o ../md5.o ../m_menu.o ../m_misc.o ../mmus2mid.o \
	../m_random.o ../p_ceilng.o ../p_checksum.o ../p_doors.o ../p_enemy.o ../p_floor.o \
	../p_genlin.o ../p_inter.o ../p_lights.o ../p_map.o ../p_maputl.o ../p_mobj.o \
	../p_plats.o ../p_pspr.o ../p_saveg.o ../p_setup.o ../p_sight.o ../p_spec.o \
	../p_switch.o ../p_telept.o ../p_tick.o ../p_user.o ../r_bsp.o ../r_data.o \
	../r_demo.o ../r_draw.o ../r_filter.o ../r_fps.o ../r_main.o ../r_patch.o \
	../r_plane.o ../r_segs.o ../r_sky.o ../r_things.o ../sounds.o ../s_sound.o \
	../st_lib.o ../st_stuff.o ../tables.o ../version.o ../v_video.o ../wi_stuff.o \
	../w_mmap.o ../w_wad.o ../z_bmalloc.o ../z_zone.o \
	../../prboom-wad-tables/GAMMATBL.o \
	../../prboom-wad-tables/SINETABL.o \
	../../prboom-wad-tables/TANGTABL.o \
	../../prboom-wad-tables/TANTOANG.o \
	i_joy.o i_main.o i_network.o i_sound.o i_system.o i_video.o

CFLAGS := -I../include -Iinclude -I../../prboom-wad-tables/include -ggdb
LDFLAGS := -lm -ggdb

doom: $(OBJS)
	$(CC) -o doom $(LDFLAGS) $(OBJS)

clean:
	rm -f doom *.o ../*.o