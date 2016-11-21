RM=rm -rf
CC=gcc

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
H_SRCS=$(shell find . -iname "*.h" | tr '\n' ' ')
OBJS=$(C_SRCS:./%.c=%.o)

# Clean and compile .so
all: build/liblibTeamRocket.so

build/liblibTeamRocket.so: $(OBJS)
	$(CC) -shared -o "liblibTeamRocket.so" $(OBJS)

%.o: ./%.c
	$(CC) -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<" -lcommons -lpthread

clean:
	$(RM) ./*.o
	$(RM) ./*.d
	$(RM) ./liblibTeamRocket.so
	

install: all
	cp -u ./liblibTeamRocket.so /usr/lib
	mkdir -p /usr/include/teamRocket/
	cp -u $(H_SRCS) /usr/include/teamRocket/

uninstall: clean
	rm -f /usr/lib/liblibTeamRocket.so
	rm -rf /usr/include/teamRocket

.PHONY: all clean install uninstall