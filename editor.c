#include "editor.h"
#include "document.h"
#include "format.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

void free_lines(int count, char **lines);

char *get_filename(int argc, char *argv[]) {
	// TODO implement get_filename
	// take a look at editor_main.c to see what this is used for
	if(argc == 2) {
		return argv[1];
	} else {
		print_usage_error();
	}
	return NULL;
}

void handle_display_command(Document *document, const char *command) {
	// TODO implement handle_display_command
	if(document == NULL || Document_size(document) == 0) {
		print_document_empty_error();
		return;
	}
	if(command == NULL) {
		return;
	}
	char cmd = 0;
	char space = 0;
	int line_number = -1;
	char last = 0;
	int size = (int)Document_size(document);
	int parsed = sscanf(command, "%c%c%d%c", &cmd, &space, &line_number, &last);
	if(parsed == 1 && strlen(command) == 1 && cmd == 'p') {
		//print whole document
		int i = 0;
		for(i = 0; i < size; i++) {
			print_line(document, i);
		}
	} else if(parsed == 3 && cmd == 'p' && space == ' ' && line_number >= 1 && last == 0) {
		//print 5 above 5 below
		if(!(line_number >= 1 && line_number <= size)) {
			invalid_line();
			return;
		}
		int index = line_number - 1;
		int start_index = max(index - 5, 0);
		int end_index = min(index + 5, size - 1);
		int i = 0;
		for(i = start_index; i <= end_index; i++) {
			print_line(document, i);
		}
	} else {
		invalid_command(command);
	}
}

void handle_write_command(Document *document, const char *command) {
	// TODO implement handle_write_command
	if(document == NULL || command == NULL) {
		return;
	}
	char cmd = 0;
	char space1 = 0;
	int line_number = -1;
	char space2 = 0;
	int size = (int)Document_size(document);
	int str_size = strlen(command) + 1;
	char buffer[str_size];
	memset(buffer, 0, str_size);
	int parsed = sscanf(command, "%c%c%d%c%[^\n]s", &cmd, &space1, &line_number, &space2, buffer);
	if((parsed == 4 || parsed == 5) && cmd == 'w' && space1 == ' ' && line_number >= 1 && space2 == ' ') {
		size_t tokens = 0;
		char **multi_line = strsplit(buffer, "$", &tokens);
		int index = line_number - 1;
		if(line_number >= 1 && line_number <= size) {
			Document_set_line(document, index, multi_line[0]);
		} else {
			Document_insert_line(document, index, multi_line[0]);
		}
		size_t i = 0;
		for(i = 1; i < tokens; i++) {
			Document_insert_line(document, index + i, multi_line[i]);
		}
		free_lines(tokens, multi_line);
	} else {
		invalid_command(command);
	}
}

void handle_append_command(Document *document, const char *command) {
	// TODO implement handle_append_command
	if(document == NULL || command == NULL) {
		return;
	}
	char cmd = 0;
	char space1 = 0;
	int line_number = -1;
	char space2 = 0;
	int size = (int)Document_size(document);
	int str_size = strlen(command) + 1;
	char buffer[str_size];
	memset(buffer, 0, str_size);
	int parsed = sscanf(command, "%c%c%d%c%[^\n]s", &cmd, &space1, &line_number, &space2, buffer);
	if((parsed == 4 || parsed == 5) && cmd == 'a' && space1 == ' ' && line_number >= 1 && space2 == ' ') {
		size_t tokens = 0;
		char **multi_line = strsplit(buffer, "$", &tokens);
		int index = line_number - 1;
		if(line_number >= 1 && line_number <= size) {
			const char *curr_str = Document_get_line(document, index);
			int new_str_size = strlen(curr_str) + strlen(buffer) + 1;
			char new_str[new_str_size];
			strcpy(new_str, curr_str);
			strcat(new_str, buffer);
			Document_set_line(document, index, new_str);
		} else {
			Document_insert_line(document, index, buffer);
		}
		size_t i = 0;
		for(i = 1; i < tokens; i++) {
			Document_insert_line(document, index + i, multi_line[i]);
		}
		free_lines(tokens, multi_line);
	} else {
		invalid_command(command);
	}
}

void handle_delete_command(Document *document, const char *command) {
	// TODO implement handle_delete_command
	if(document == NULL || command == NULL) {
		return;
	}
	char cmd = 0;
	char space = 0;
	int line_number = -1;
	char last = 0;
	int size = (int)Document_size(document);
	int parsed = sscanf(command, "%c%c%d%c", &cmd, &space, &line_number, &last);
	if(parsed == 3 && cmd == 'd' && space == ' ' && line_number >= 1 && last == 0) {
		if(!(line_number >= 1 && line_number <= size)) {
			invalid_line();
			return;
		}
		int index = line_number - 1;
		Document_delete_line(document, index);
	} else {
		invalid_command(command);
	}
}

void handle_search_command(Document *document, const char *command) {
	// TODO implement handle_search_command
	if(document == NULL || command == NULL) {
		return;
	}
	char cmd = 0;
	int str_size = strlen(command) + 1;
	char buffer[str_size];
	memset(buffer, 0, str_size);
	int parsed = sscanf(command, "%c%[^\n]s", &cmd, buffer);
	if(parsed == 1 && strlen(command) == 1 && cmd == '/') {
		return;
	} else if(parsed == 2 && cmd == '/') {
		size_t index = 0;
		for(index = 0; index < Document_size(document); index++) {
			const char *str = Document_get_line(document, index);
			const char *start = NULL;
			if((start = strstr(str, buffer)) != NULL) {
				print_search_line(index + 1, str, start, buffer);
			}
		}
	} else {
		invalid_command(command);
	}
}

void handle_save_command(Document *document, const char *filename) {
	// TODO implement handle_save_command
	if(document == NULL || filename == NULL) {
		return;
	}
	Document_write_to_file(document, filename);
}

void free_lines(int count, char **lines) {
    if(lines == NULL) {
        return;
    }
    int i = 0;
    for(i = 0; i < count; i++) {
    	free(lines[i]);
    }
    free(lines);
}