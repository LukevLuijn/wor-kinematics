ACLOCAL_AMFLAGS = -I ./m4
SUBDIRS = src

if HAVE_CPPCHECK
cppcheck:
		find ../ -type f -name "*.cpp" | $(CPPCHECK) 	--quiet \
							--template="{file};{line};{severity};{id};{message}" \
							--enable=all \
							--inconclusive	\
							--force --inline-suppr	\
							--std=c11 --std=c++20	\
							--suppress=missingIncludeSystem	\
							--file-list=-
														
cppcheck_checkconfig:														
		find ../ -type f -name "*.cpp" | $(CPPCHECK) 	--quiet \
							--template="{file};{line};{severity};{id};{message}" \
							--enable=all \
							--inconclusive	\
							--force --inline-suppr	\
							--std=c11 --std=c++20	\
							--suppress=missingIncludeSystem	\
							--check-config	\
							--file-list=-
endif		

if HAVE_DOXYGEN
doxygen:
		mkdir -p ../docs/generated
		$(DOXYGEN) ../Doxyfile
endif

if HAVE_CLANGTIDY
clangtidy:
		$(CLANGTIDY)
endif

if HAVE_BEAR
bear:
		make clean
		$(BEAR) -- make
endif

if HAVE_COMPILEDB
compiledb:
		make clean
		$(COMPILEDB) -n make --dry-run
endif

