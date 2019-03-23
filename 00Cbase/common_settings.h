#ifndef COMMON_SETTINGS_H_
#define COMMON_SETTINGS_H_

//extern int DEBUG;

void check_debug_env_set(void);

int is_debug_enabled();

int is_tty_echo_enabled();

void check_tty_echo_off_env_set(void);

void set_tty_echo(int set_echo);

int my_getchar();

int prefix(const char *pre, const char *str);

#endif /* COMMON_SETTINGS_H_ */
