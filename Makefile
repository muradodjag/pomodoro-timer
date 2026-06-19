CC         = gcc
TARGET     = pomodoro-timer
SRCS       = main.c timer.c
CFLAGS     = $(shell pkg-config --cflags raylib libnotify)
LIBS       = $(shell pkg-config --libs raylib libnotify)

INSTALL_BIN  = $(HOME)/.local/bin
DESKTOP_DIR  = $(HOME)/.local/share/applications
SCRIPT_DIR   = /usr/local/bin

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LIBS)

install: $(TARGET)
	mkdir -p $(INSTALL_BIN) $(DESKTOP_DIR)
	cp $(TARGET) $(INSTALL_BIN)/$(TARGET)
	@printf '[Desktop Entry]\nName=Pomodoro Timer\nExec=%s/%s\nType=Application\nCategories=Utility;\nTerminal=false\n' \
		$(INSTALL_BIN) $(TARGET) > $(DESKTOP_DIR)/$(TARGET).desktop
	@echo "Installed. Run 'make sudoers' once to enable site blocking."

sudoers:
	sudo install -o root -g root -m 755 scripts/pomodoro-block   $(SCRIPT_DIR)/pomodoro-block
	sudo install -o root -g root -m 755 scripts/pomodoro-unblock $(SCRIPT_DIR)/pomodoro-unblock
	@echo "$(USER) ALL=(ALL) NOPASSWD: $(SCRIPT_DIR)/pomodoro-block, $(SCRIPT_DIR)/pomodoro-unblock" \
		| sudo tee /etc/sudoers.d/pomodoro-timer
	@sudo chmod 440 /etc/sudoers.d/pomodoro-timer
	@echo "Done. Site blocking is now passwordless."

uninstall:
	rm -f $(INSTALL_BIN)/$(TARGET)
	rm -f $(DESKTOP_DIR)/$(TARGET).desktop
	sudo rm -f $(SCRIPT_DIR)/pomodoro-block
	sudo rm -f $(SCRIPT_DIR)/pomodoro-unblock
	sudo rm -f /etc/sudoers.d/pomodoro-timer

clean:
	rm -f $(TARGET)

.PHONY: install sudoers uninstall clean
