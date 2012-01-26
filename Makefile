# Alternate way of building node-talib module
# Similar results can be achieved with npm

all:
	@cd ./src/lib && ./configure && make && cd ../../ && node-waf configure && node-waf build
	@echo
	@echo build complete
	
clean:
	@rm -rf build/* && cd ./src/lib && make maintainer-clean && cd ../../
	@echo
	@echo clean complete