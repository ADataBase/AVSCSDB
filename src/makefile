# makefile for scons and stuff

.PHONY: sconsfailtest sconslib sconsample test .PHONY all

all: scons

cls:
	alias cls="clear"
	cls || clear

scons: sconsanity sconsfailtest sconslib sconsample

clean:
	scons -c CleanCWD=0
	rm *.o || echo "Nah"
	rm *.exe || echo "Nah"
	rm bin/*.o || echo "Nah"
	rm bin/*.exe || echo "Nah"

sconsanity:
	@echo "This is a test to see if the working enviorment is set up properly, if this pass, the it is good to go"
	scons $(SCONSFLAG) -Q SanityCheck=0

sconslib:
	@echo "This is a library generated as an object file"
	scons $(SCONSFLAG) -Q ObjectLib=0

sconsample:
	@echo "This is a sample program"
	scons $(SCONSFLAG) -Q SampleProgram=0

sconsfailtest:
	@echo "If sconsfailtest fail-test fails, it means there is an error in the code"
	scons $(SCONSFLAG) -Q FailTestProgram=0

test: sconsfailtest