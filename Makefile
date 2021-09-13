all:
	gcc tag.c -o tag
	sudo mv tag /usr/bin
	touch hierarchy
	sudo mv hierarchy /usr/bin