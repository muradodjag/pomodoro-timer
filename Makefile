CC         = gcc
TARGET     = pomodoro-timer
SRCS       = main.c timer.c
CFLAGS     = $(shell pkg-config --cflags raylib libnotify)
LIBS       = $(shell pkg-config --libs raylib libnotify)

INSTALL_BIN  = $(HOME)/.local/bin
DESKTOP_DIR  = $(HOME)/.local/share/applications

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LIBS)

install: $(TARGET)
	mkdir -p $(INSTALL_BIN) $(DESKTOP_DIR)
	cp $(TARGET) $(INSTALL_BIN)/$(TARGET)
	@printf '[Desktop Entry]\nName=Pomodoro Timer\nExec=%s/%s\nType=Application\nCategories=Utility;\nTerminal=false\n' \
		$(INSTALL_BIN) $(TARGET) > $(DESKTOP_DIR)/$(TARGET).desktop
	@echo "Installed. Open Walker and search 'Pomodoro Timer'."

uninstall:
	rm -f $(INSTALL_BIN)/$(TARGET)
	rm -f $(DESKTOP_DIR)/$(TARGET).desktop

clean:
	rm -f $(TARGET)

.PHONY: install uninstall clean
