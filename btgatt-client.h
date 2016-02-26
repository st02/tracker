#ifndef BTGATT_CLIENT_H
#define BTGATT_CLIENT_H

#define COLOR_OFF	"\x1B[0m"
#define COLOR_RED	"\x1B[0;91m"
#define COLOR_GREEN	"\x1B[0;92m"
#define COLOR_YELLOW	"\x1B[0;93m"
#define COLOR_BLUE	"\x1B[0;94m"
#define COLOR_MAGENTA	"\x1B[0;95m"
#define COLOR_BOLDGRAY	"\x1B[1;30m"
#define COLOR_BOLDWHITE	"\x1B[1;37m"

#define PRLOG(...) \
	printf(__VA_ARGS__); print_prompt();

struct client {
	int fd;
	struct bt_att *att;
	struct gatt_db *db;
	struct bt_gatt_client *gatt;

	unsigned int reliable_session_id;
};

void print_prompt(void);
void cmd_write_value(struct client *cli, char *cmd_str);
void cmd_read_value(struct client *cli, char *cmd_str);
void cmd_register_notify(struct client *cli, char *cmd_str);
void cmd_read_multiple(struct client *cli, char *cmd_str);
FILE *fp;

#endif
