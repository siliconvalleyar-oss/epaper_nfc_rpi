# Makefile principal - Build all epaper projects
# Usage: make clean  (limpia todos los proyectos)
#        make        (compila todos los proyectos)
#        make -j4    (compila en paralelo)

PROJECTS = epaper_nfc_src
.PHONY: all clean run $(PROJECTS)

all: $(PROJECTS)

$(PROJECTS):
	@echo "========================================"
	@echo "Building: $@"
	@echo "========================================"
	$(MAKE) -C $@
	@echo ""

clean:
	@echo "========================================"
	@echo "Cleaning all projects..."
	@echo "========================================"
	@for proj in $(PROJECTS); do \
		echo "Cleaning: $$proj"; \
		$(MAKE) -C $$proj clean; \
	done
	@echo "All projects cleaned."

run:
	@for proj in $(PROJECTS); do \
		echo "Running: $$proj"; \
		$(MAKE) -C $$proj run; \
	done

.PHONY: help
help:
	@echo "Targets:"
	@echo "  all     - Build all projects (default)"
	@echo "  clean   - Clean all projects"
	@echo "  run     - Build (if needed) and run all projects"
	@echo "  help    - Show this help"
	@echo ""
	@echo "Projects: $(PROJECTS)"
