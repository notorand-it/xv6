U = user
ULIB = $(U)/ulib.o $(U)/usys.o $(U)/printf.o $(U)/umalloc.o
<<<<<<< HEAD

=======
>>>>>>> 3d8cc15bd3e487d6b565c410978045389f51f0f2
$U/_%: proj1/user/%.o $(ULIB)
	$(LD) $(LDFLAGS) -T $U/user.ld -o $@ $^
	$(OBJDUMP) -S $@ > $U/$*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $U/$*.sym
