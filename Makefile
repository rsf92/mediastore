################################################### UCONSTANTS ##################################################################
CC = @gcc
CFLAGS = -c -g -Wall -pedantic -std=gnu11 -Wextra -Wfloat-equal -Wundef -Wconversion 	-Wunreachable-code -Wstrict-prototypes -I/usr/include/postgresql/
LINKERS = -lpthread -lpq
EXECUTABLES= mediaServer $(TESTS)
TESTS = test_util test_user test_cmdstd
OBJDIR = obj/
OBJS = $(OBJDIR)service.o $(OBJDIR)config.o $(OBJDIR)fschecks.o $(OBJDIR)fsconfig.o $(OBJDIR)dbstd.o \
		$(OBJDIR)dbuser.o $(OBJDIR)bootup_server.o $(OBJDIR)main.o $(OBJDIR)fshandle.o $(OBJDIR)util.o $(OBJDIR)cmdstd.o $(OBJDIR)user.o
.SUFFIXES: .o .c

################################################### MAIN ACTIONS ##################################################################
recompile: clean all

all: $(EXECUTABLES)
################################################### EXECUTABLES ##################################################################
mediaServer:  $(OBJS)
	$(CC) $^ -o $@ $(LINKERS)
################################################### MAIN FUNCTIONS ##################################################################
$(OBJDIR)main.o: src/main.c
	$(CC) $(CFLAGS) src/main.c -o $@
################################################### UNIT TESTS ##################################################################
test_util: $(OBJDIR)util.o $(OBJDIR)test_util.o
			$(CC) $^ -o $@
test_user: $(OBJDIR)util.o $(OBJDIR)user.o $(OBJDIR)test_user.o
			$(CC) $^ -o $@
test_cmdstd: $(OBJDIR)cmdstd.o $(OBJDIR)test_cmdstd.o $(OBJDIR)util.o
				$(CC) $^ -o $@
################################################### FILE SYSTEM FUNCTIONS ##################################################################
$(OBJDIR)fshandle.o: src/fs/fshandle.c include/fs/fshandle.h
					$(CC) $(CFLAGS) src/fs/fshandle.c -o $@
################################################### UTILITIES FUNCTIONS ##################################################################
$(OBJDIR)util.o: src/util/util.c include/util/util.h
		$(CC) $(CFLAGS) src/util/util.c -o $@
################################################### BASIC ATD FUNCTIONS ##################################################################
$(OBJDIR)user.o: src/user/user.c include/user/user.h
		$(CC) $(CFLAGS) src/user/user.c -o $@
################################################### COMMAND PROCESSING FUNCTIONS ##################################################################
$(OBJDIR)cmdstd.o: src/cmd/cmdstd.c include/cmd/cmdstd.h
		$(CC) $(CFLAGS) src/cmd/cmdstd.c -o $@
################################################### TEST FUNCTIONS ##################################################################
$(OBJDIR)test_util.o: test/util/test_util.c
		$(CC) $(CFLAGS) test/util/test_util.c -o $@
$(OBJDIR)test_user.o: test/user/test_user.c
		$(CC) $(CFLAGS) $^  -o $@
$(OBJDIR)test_cmdstd.o: test/cmd/test_cmdstd.c
		$(CC) $(CFLAGS) $^  -o $@
################################################### SERVER NETWORKING ##################################################################
$(OBJDIR)bootup_server.o: src/network/bootup_server.c include/network/bootup_server.h
		$(CC) $(CFLAGS) src/network/bootup_server.c -o $@
$(OBJDIR)service.o: src/network/service.c include/network/service.h
	$(CC) $(CFLAGS) src/network/service.c -o $@
################################################### FILE SYSTEM FUNCTIONS ##################################################################
$(OBJDIR)fschecks.o: src/fs/fschecks.c include/fs/fschecks.h
		$(CC) $(CFLAGS) src/fs/fschecks.c -o $@
$(OBJDIR)fsconfig.o: src/fs/fsconfig.c include/fs/fsconfig.h
	$(CC) $(CFLAGS) src/fs/fsconfig.c -o $@
################################################### CONFIG FILE FUNCTIONS ##################################################################
$(OBJDIR)config.o: src/config/config.c include/config/config.h
	$(CC) $(CFLAGS) src/config/config.c -o $@
################################################### DATABASE FUNCTIONS ##################################################################
$(OBJDIR)dbstd.o: src/db/dbstd.c include/db/dbstd.h
	$(CC) $(CFLAGS) src/db/dbstd.c -o $@
$(OBJDIR)dbuser.o: src/db/dbuser.c include/db/dbuser.h
	$(CC) $(CFLAGS) src/db/dbuser.c -o $@
################################################### OTHER OPERATIONS ##################################################################
clean:
	rm -f $(OBJS) $(EXECUTABLES)

tests: $(TESTS)
		@echo "Running test for util module"
		@./test_util
		@echo "Running test for user module"
		@./test_user
		@echo "Running test for cmdstd module"
		@./test_cmdstd
		@echo "Running test for locale module"
		@./test_locale
