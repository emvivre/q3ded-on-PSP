TARGET=q3ded
PSP_EBOOT_TITLE = 'Quake 3 server dedicated'

CC=psp-gcc
INCDIR =
CFLAGS = -O2 -G0 -Wall -fno-strict-aliasing -DQ3_STATIC -DGAME_HARD_LINKED
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LDFLAGS =
LIBDIR = 
LIBS = -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpspnet -lpspnet_inet -lpspnet_apctl -lpspnet_resolver -lpsputility -lpspuser -lpspkernel -lpspwlan -lm -lpsppower

USE_PSPSDK_LIBC=1


B=bin
SDIR=server
CMDIR=qcommon
BLIBDIR=botlib
UDIR=unix
NDIR=null
GDIR=game
DO_DED_CC=$(CC) -DDEDICATED -DC_ONLY $(CFLAGS) -o $@ -c $<
DO_BOT_CC=$(CC) $(CFLAGS) -DBOTLIB  -o $@ -c $<


OBJS = \
	$(B)/sv_bot.o \
	$(B)/sv_ccmds.o \
	$(B)/sv_game.o \
	$(B)/sv_init.o \
	$(B)/sv_main.o \
	$(B)/sv_net_chan.o \
	$(B)/sv_snapshot.o \
	$(B)/sv_world.o \
	$(B)/cm_load.o \
	$(B)/cm_patch.o \
	$(B)/cm_polylib.o \
	$(B)/cm_test.o \
	$(B)/cm_trace.o \
	$(B)/cmd.o \
	$(B)/common.o \
	$(B)/cvar.o \
	$(B)/files.o \
	$(B)/md4.o \
	$(B)/msg.o \
	$(B)/huffman.o \
	$(B)/q_math.o \
	$(B)/q_shared.o \
	$(B)/unzip.o \
	$(B)/vm.o \
	$(B)/vm_interpreted.o \
	$(B)/be_aas_bspq3.o \
	$(B)/be_aas_cluster.o \
	$(B)/be_aas_debug.o \
	$(B)/be_aas_entity.o \
	$(B)/be_aas_file.o \
	$(B)/be_aas_main.o \
	$(B)/be_aas_move.o \
	$(B)/be_aas_optimize.o \
	$(B)/be_aas_reach.o \
	$(B)/be_aas_route.o \
	$(B)/be_aas_routealt.o \
	$(B)/be_aas_sample.o \
	$(B)/be_ai_char.o \
	$(B)/be_ai_chat.o \
	$(B)/be_ai_gen.o \
	$(B)/be_ai_goal.o \
	$(B)/be_ai_move.o \
	$(B)/be_ai_weap.o \
	$(B)/be_ai_weight.o \
	$(B)/be_ea.o \
	$(B)/be_interface.o \
	$(B)/l_cmd.o \
	$(B)/l_crc.o \
	$(B)/l_libvar.o \
	$(B)/l_log.o \
	$(B)/l_memory.o \
	$(B)/l_precomp.o \
	$(B)/l_script.o \
	$(B)/l_struct.o \
	$(B)/unix_main.o \
	$(B)/unix_shared.o \
	$(B)/null_client.o \
	$(B)/null_input.o \
	$(B)/null_snddma.o \
	$(B)/sv_client.o \
	$(B)/unix_net.o \
        $(B)/net_chan.o \
	$(B)/ai_chat.o \
	$(B)/ai_cmd.o \
	$(B)/ai_dmnet.o \
	$(B)/ai_dmq3.o \
	$(B)/ai_main.o \
	$(B)/ai_team.o \
	$(B)/ai_vcmd.o \
	$(B)/bg_lib.o \
	$(B)/bg_misc.o \
	$(B)/bg_pmove.o \
	$(B)/bg_slidemove.o \
	$(B)/g_active.o \
	$(B)/g_arenas.o \
	$(B)/g_bot.o \
	$(B)/g_client.o \
	$(B)/g_cmds.o \
	$(B)/g_combat.o \
	$(B)/g_items.o \
	$(B)/g_main.o \
	$(B)/g_mem.o \
	$(B)/g_misc.o \
	$(B)/g_missile.o \
	$(B)/g_mover.o \
	$(B)/g_session.o \
	$(B)/g_spawn.o \
	$(B)/g_svcmds.o \
	$(B)/g_syscalls.o \
	$(B)/g_target.o \
	$(B)/g_team.o \
	$(B)/g_trigger.o \
	$(B)/g_utils.o \
	$(B)/g_weapon.o 

#	$(B)/linux_common.o \	#fonctions deja defini 
#	$(B)/g_rankings.o \

$(B)/$(TARGET) : $(OBJS)
	$(CC)  -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIBS)
	psp-fixup-imports $@
	mksfo $(PSP_EBOOT_TITLE) PARAM.SFO
	psp-strip $@ -o $@_strip.elf
	pack-pbp EBOOT.PBP PARAM.SFO NULL  \
	NULL NULL NULL  \
	NULL  $@_strip.elf NULL

$(B)/sv_bot.o : $(SDIR)/sv_bot.c; $(DO_DED_CC) 
$(B)/sv_client.o : $(SDIR)/sv_client.c; $(DO_DED_CC) 
$(B)/sv_ccmds.o : $(SDIR)/sv_ccmds.c; $(DO_DED_CC) 
$(B)/sv_game.o : $(SDIR)/sv_game.c; $(DO_DED_CC) 
$(B)/sv_init.o : $(SDIR)/sv_init.c; $(DO_DED_CC) 
$(B)/sv_main.o : $(SDIR)/sv_main.c; $(DO_DED_CC) 
$(B)/sv_net_chan.o : $(SDIR)/sv_net_chan.c; $(DO_DED_CC) 
$(B)/sv_snapshot.o : $(SDIR)/sv_snapshot.c; $(DO_DED_CC) 
$(B)/sv_world.o : $(SDIR)/sv_world.c; $(DO_DED_CC) 
$(B)/cm_load.o : $(CMDIR)/cm_load.c; $(DO_DED_CC) 
$(B)/cm_polylib.o : $(CMDIR)/cm_polylib.c; $(DO_DED_CC) 
$(B)/cm_test.o : $(CMDIR)/cm_test.c; $(DO_DED_CC) 
$(B)/cm_trace.o : $(CMDIR)/cm_trace.c; $(DO_DED_CC) 

$(B)/cm_patch.o : $(CMDIR)/cm_patch.c; $(DO_DED_CC) 
$(B)/cmd.o : $(CMDIR)/cmd.c; $(DO_DED_CC) 
$(B)/common.o : $(CMDIR)/common.c; $(DO_DED_CC) 
$(B)/cvar.o : $(CMDIR)/cvar.c; $(DO_DED_CC) 
$(B)/files.o : $(CMDIR)/files.c; $(DO_DED_CC) 
$(B)/md4.o : $(CMDIR)/md4.c; $(DO_DED_CC) 
$(B)/msg.o : $(CMDIR)/msg.c; $(DO_DED_CC) 
$(B)/net_chan.o : $(CMDIR)/net_chan.c; $(DO_DED_CC)
$(B)/huffman.o : $(CMDIR)/huffman.c; $(DO_DED_CC)
$(B)/q_shared.o : $(GDIR)/q_shared.c; $(DO_DED_CC) 
$(B)/q_math.o : $(GDIR)/q_math.c; $(DO_DED_CC) 
$(B)/ai_chat.o :  $(GDIR)/ai_chat.c; $(DO_DED_CC) 
$(B)/ai_cmd.o :  $(GDIR)/ai_cmd.c; $(DO_DED_CC) 
$(B)/ai_dmnet.o :  $(GDIR)/ai_dmnet.c; $(DO_DED_CC) 
$(B)/ai_dmq3.o :  $(GDIR)/ai_dmq3.c; $(DO_DED_CC) 
$(B)/ai_main.o :  $(GDIR)/ai_main.c; $(DO_DED_CC)
$(B)/ai_team.o :  $(GDIR)/ai_team.c; $(DO_DED_CC)  
$(B)/ai_vcmd.o :  $(GDIR)/ai_vcmd.c; $(DO_DED_CC) 
$(B)/bg_lib.o :  $(GDIR)/bg_lib.c; $(DO_DED_CC) 
$(B)/bg_misc.o :  $(GDIR)/bg_misc.c; $(DO_DED_CC) 
$(B)/bg_pmove.o :  $(GDIR)/bg_pmove.c; $(DO_DED_CC) 
$(B)/bg_slidemove.o :  $(GDIR)/bg_slidemove.c; $(DO_DED_CC) 
$(B)/g_active.o :  $(GDIR)/g_active.c; $(DO_DED_CC) 
$(B)/g_arenas.o :  $(GDIR)/g_arenas.c; $(DO_DED_CC)
$(B)/g_bot.o :  $(GDIR)/g_bot.c; $(DO_DED_CC)
$(B)/g_client.o :  $(GDIR)/g_client.c; $(DO_DED_CC)
$(B)/g_cmds.o :  $(GDIR)/g_cmds.c; $(DO_DED_CC)
$(B)/g_combat.o :  $(GDIR)/g_combat.c; $(DO_DED_CC)
$(B)/g_items.o :  $(GDIR)/g_items.c; $(DO_DED_CC)
$(B)/g_main.o :  $(GDIR)/g_main.c; $(DO_DED_CC)
$(B)/g_mem.o :  $(GDIR)/g_mem.c; $(DO_DED_CC)
$(B)/g_misc.o :  $(GDIR)/g_misc.c; $(DO_DED_CC)
$(B)/g_missile.o :  $(GDIR)/g_missile.c; $(DO_DED_CC)
$(B)/g_mover.o :  $(GDIR)/g_mover.c; $(DO_DED_CC)
$(B)/g_rankings.o :  $(GDIR)/g_rankings.c; $(DO_DED_CC)
$(B)/g_session.o :  $(GDIR)/g_session.c; $(DO_DED_CC)
$(B)/g_spawn.o :  $(GDIR)/g_spawn.c; $(DO_DED_CC)
$(B)/g_svcmds.o :  $(GDIR)/g_svcmds.c; $(DO_DED_CC)
$(B)/g_syscalls.o :  $(GDIR)/g_syscalls.c; $(DO_DED_CC)
$(B)/g_target.o : $(GDIR)/g_target.c; $(DO_DED_CC)
$(B)/g_team.o : $(GDIR)/g_team.c; $(DO_DED_CC)
$(B)/g_trigger.o : $(GDIR)/g_trigger.c; $(DO_DED_CC)
$(B)/g_utils.o : $(GDIR)/g_utils.c; $(DO_DED_CC)
$(B)/g_weapon.o : $(GDIR)/g_weapon.c; $(DO_DED_CC)
$(B)/be_aas_bspq3.o : $(BLIBDIR)/be_aas_bspq3.c; $(DO_BOT_CC) 
$(B)/be_aas_cluster.o : $(BLIBDIR)/be_aas_cluster.c; $(DO_BOT_CC) 
$(B)/be_aas_debug.o : $(BLIBDIR)/be_aas_debug.c; $(DO_BOT_CC) 
$(B)/be_aas_entity.o : $(BLIBDIR)/be_aas_entity.c; $(DO_BOT_CC) 
$(B)/be_aas_file.o : $(BLIBDIR)/be_aas_file.c; $(DO_BOT_CC) 
$(B)/be_aas_main.o : $(BLIBDIR)/be_aas_main.c; $(DO_BOT_CC) 
$(B)/be_aas_move.o : $(BLIBDIR)/be_aas_move.c; $(DO_BOT_CC) 
$(B)/be_aas_optimize.o : $(BLIBDIR)/be_aas_optimize.c; $(DO_BOT_CC) 
$(B)/be_aas_reach.o : $(BLIBDIR)/be_aas_reach.c; $(DO_BOT_CC)
$(B)/be_aas_route.o : $(BLIBDIR)/be_aas_route.c; $(DO_BOT_CC) 
$(B)/be_aas_routealt.o : $(BLIBDIR)/be_aas_routealt.c; $(DO_BOT_CC) 
$(B)/be_aas_sample.o : $(BLIBDIR)/be_aas_sample.c; $(DO_BOT_CC) 
$(B)/be_ai_char.o : $(BLIBDIR)/be_ai_char.c; $(DO_BOT_CC) 
$(B)/be_ai_chat.o : $(BLIBDIR)/be_ai_chat.c; $(DO_BOT_CC) 
$(B)/be_ai_gen.o : $(BLIBDIR)/be_ai_gen.c; $(DO_BOT_CC) 
$(B)/be_ai_goal.o : $(BLIBDIR)/be_ai_goal.c; $(DO_BOT_CC) 
$(B)/be_ai_move.o : $(BLIBDIR)/be_ai_move.c; $(DO_BOT_CC) 
$(B)/be_ai_weap.o : $(BLIBDIR)/be_ai_weap.c; $(DO_BOT_CC) 
$(B)/be_ai_weight.o : $(BLIBDIR)/be_ai_weight.c; $(DO_BOT_CC) 
$(B)/be_ea.o : $(BLIBDIR)/be_ea.c; $(DO_BOT_CC) 
$(B)/be_interface.o : $(BLIBDIR)/be_interface.c; $(DO_BOT_CC) 
$(B)/l_cmd.o : bspc/l_cmd_modified.c; $(DO_BOT_CC) 
$(B)/l_crc.o : $(BLIBDIR)/l_crc.c; $(DO_BOT_CC) 
$(B)/l_libvar.o : $(BLIBDIR)/l_libvar.c; $(DO_BOT_CC) 
$(B)/l_log.o : $(BLIBDIR)/l_log.c; $(DO_BOT_CC) 
$(B)/l_memory.o : $(BLIBDIR)/l_memory.c; $(DO_BOT_CC) 
$(B)/l_precomp.o : $(BLIBDIR)/l_precomp.c; $(DO_BOT_CC) 
$(B)/l_script.o : $(BLIBDIR)/l_script.c; $(DO_BOT_CC) 
$(B)/l_struct.o : $(BLIBDIR)/l_struct.c; $(DO_BOT_CC) 

$(B)/linux_common.o : $(UDIR)/linux_common.c; $(DO_DED_CC)
$(B)/unix_main.o : $(UDIR)/unix_main.c; $(DO_DED_CC)
$(B)/unix_net.o : $(UDIR)/unix_net.c; $(DO_DED_CC) 
$(B)/unix_shared.o : $(UDIR)/unix_shared.c; $(DO_DED_CC) 
$(B)/null_client.o : $(NDIR)/null_client.c; $(DO_DED_CC) 
$(B)/null_input.o : $(NDIR)/null_input.c; $(DO_DED_CC) 
$(B)/null_snddma.o : $(NDIR)/null_snddma.c; $(DO_DED_CC) 
$(B)/unzip.o : $(CMDIR)/unzip.c; $(DO_DED_CC) 
$(B)/vm.o : $(CMDIR)/vm.c; $(DO_DED_CC) 
$(B)/vm_interpreted.o : $(CMDIR)/vm_interpreted.c; $(DO_DED_CC) 


EXTRA_TARGETS = EBOOT.PBP


PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

