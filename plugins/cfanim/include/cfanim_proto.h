/* cfanim.c */
int get_dir_from_name(char *name);
long int initmovement(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runmovement(struct CFanimation_struct *animation, long int id, void *parameters);
long int initfire(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runfire(struct CFanimation_struct *animation, long int id, void *parameters);
long int initturn(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runturn(struct CFanimation_struct *animation, long int id, void *parameters);
long int initcamera(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runcamera(struct CFanimation_struct *animation, long int id, void *parameters);
long int initvisible(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runvisible(struct CFanimation_struct *animation, long int id, void *parameters);
long int initwizard(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runwizard(struct CFanimation_struct *animation, long int id, void *parameters);
long int initsay(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runsay(struct CFanimation_struct *animation, long int id, void *parameters);
long int initapply(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runapply(struct CFanimation_struct *animation, long int id, void *parameters);
long int initapplyobject(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runapplyobject(struct CFanimation_struct *animation, long int id, void *parameters);
long int initdropobject(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result rundropobject(struct CFanimation_struct *animation, long int id, void *parameters);
long int initpickup(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runpickup(struct CFanimation_struct *animation, long int id, void *parameters);
long int initpickupobject(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runpickupobject(struct CFanimation_struct *animation, long int id, void *parameters);
long int initghosted(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runghosted(struct CFanimation_struct *animation, long int id, void *parameters);
long int initteleport(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runteleport(struct CFanimation_struct *animation, long int id, void *parameters);
long int initnotice(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runnotice(struct CFanimation_struct *animation, long int id, void *parameters);
long int initstop(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runstop(struct CFanimation_struct *animation, long int id, void *parameters);
long int initmoveto(char *name, char *parameters, struct CFmovement_struct *move_entity);
anim_move_result runmoveto(struct CFanimation_struct *animation, long int id, void *parameters);
void prepare_commands(void);
int equality_split(char *buffer, char **variable, char **value);
int get_boolean(char *strg, int *bl);
int is_animated_player(object *pl);
int start_animation(object *who, object *activator, object *event, char *file, char *options);
void animate(void);
void initContextStack(void);
void pushContext(CFPContext *context);
CFPContext *popContext(void);
CF_PLUGIN int initPlugin(const char *iversion, f_plug_api gethooksptr);
CF_PLUGIN void *getPluginProperty(int *type, ...);
CF_PLUGIN anim_move_result runPluginCommand(object *op, char *params);
CF_PLUGIN int postInitPlugin(void);
CF_PLUGIN void *globalEventListener(int *type, ...);
CF_PLUGIN void *eventListener(int *type, ...);
CF_PLUGIN int closePlugin(void);
