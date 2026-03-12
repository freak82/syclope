
debug:
	cmake --build ./build/ --verbose --config Debug

reldeb:
	cmake --build ./build/ --verbose --config RelWithDebInfo

release:
	cmake --build ./build/ --verbose --config Release

config:
	cmake -B ./build -S . -G "Ninja Multi-Config" -DCMAKE_EXPORT_COMPILE_COMMANDS=1

check:
	CodeChecker analyze ./build/compile_commands.json -o ./build/code-check-reports

report:
	CodeChecker parse ./build/code-check-reports -e html -o ./build/code-check-reports/html ; \
		xdg-open ./build/code-check-reports/html/index.html

.PHONY:
	config debug reldeb release check report
