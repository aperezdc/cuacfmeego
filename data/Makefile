
INFOS := $(filter-out =template.info,$(wildcard *.info))

all: shows.json

shows.json: $(INFOS)
	./make-shows-json $^ > $@

