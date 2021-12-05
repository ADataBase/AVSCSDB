# makefile for scons and stuff

SCONSLOC=../extern/scons/scripts/scons.py

.PHONY: sconsfailtest sconslib sconsample test sample .PHONY all

all: scons

cls:
	alias cls="clear"
	cls || clear

scons: sconsanity sconsfailtest sconsample

clean:
	$(SCONSLOC) -c CleanCWD=0
	rm *.o || echo "Nah"
	rm *.exe || echo "Nah"
	rm bin/*.o || echo "Nah"
	rm bin/*.exe || echo "Nah"

sconsanity:
	@echo "This is a test to see if the working enviorment is set up properly, if this pass, the it is good to go"
	$(SCONSLOC) $(SCONSFLAG) -Q SanityCheck=0

sconsample:
	@echo "This is a sample program"
	$(SCONSLOC) $(SCONSFLAG) -Q SampleProgram=0

sconsfailtest:
	@echo "If sconsfailtest fail-test fails, it means there is an error in the code"
	$(SCONSLOC) $(SCONSFLAG) -Q FailTestProgram=0

test: sconsfailtest

sample: sconsample