first: default

PLNAME = $(shell uname)

ifeq ($(PLNAME), Linux)
	COMMAND = "\#define SVN_REV_STR \"`svn info http://svn.berlios.de/svnroot/repos/projectconcepto/trunk | grep Revision | awk '{print $$2}'`\""
else
	COMMAND = "\#define SVN_REV_STR \"`svn info | grep Revision | awk '{print $$2}'`\""
endif

svnupdate:
	@echo "---------SVN INFO GEN-----------"
	touch app/About/AboutSvnInfo.h
	rm app/About/AboutSvnInfo.h
	@echo "" >> app/About/AboutSvnInfo.h 
	@echo "#ifndef SVN_INFO_H" >> app/About/AboutSvnInfo.h 
	@echo "#define SVN_INFO_H" >> app/About/AboutSvnInfo.h 
	@echo "" >> app/About/AboutSvnInfo.h  
	@echo "//do not edit this" >> app/About/AboutSvnInfo.h
	@echo $(COMMAND) >> app/About/AboutSvnInfo.h
	@echo "" >> app/About/AboutSvnInfo.h 
	@echo "#endif" >> app/About/AboutSvnInfo.h 
	@echo "UPDATED"
	@echo "--------------------------------"

app: svnupdate app/makefile FORCE
	cd app && make

plugins: plugins/makefile FORCE
	cd plugins && make

app_clean:
	cd app && make clean

plugins_clean:
	cd plugins && make clean

app_install: app/makefile FORCE
	cd app && make install

plugins_install: plugins/makefile FORCE
	cd plugins && make install

default: app plugins
clean: app_clean plugins_clean
install: app_install plugins_install

FORCE:
