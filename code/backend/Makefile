
%.o:
	make -C $* $@

clean:
	$(foreach dir,$(shell find . -type d -maxdepth 1 -mindepth 1 | grep -v CVS),make -C $(dir) clean)

