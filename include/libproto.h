/*
 *   This file was automatically generated by version 1.7 of cextract.
 *   Manual editing not recommended.
 *
 *   Created: Fri Aug  4 17:36:36 2000
 */
#ifndef __CEXTRACT__
#ifdef __STDC__

extern int absdir ( int d );
extern void add_abilities ( object *op, object *change );
extern void add_arch ( archetype *at );
extern void add_button_link ( object *button, mapstruct *map, int connected );
extern void add_carried_lights ( object *pl );
extern void add_exp ( object *op, int exp );
extern void add_friendly_object ( object *op );
extern void add_god_to_list ( archetype *god_arch );
extern void add_light_to_list ( object *light, object *op );
extern void add_light_to_map ( object *ob, mapstruct *map );
extern char * add_refcount ( char *str );
extern void add_statbonus ( object *op );
extern char * add_string ( const char *str );
extern void add_weight ( object *op, signed long weight );
extern int adjust_exp ( object *op, int exp );
extern void allocate_map ( mapstruct *m );
extern int allowed_class ( object *op );
extern void animate_object ( object *op );
extern void animate_turning ( object *op );
extern int arch_blocked ( archetype *at, mapstruct *m, int x, int y );
extern void arch_info ( object *op );
extern int arch_out_of_map ( archetype *at, mapstruct *m, int x, int y );
extern object *arch_to_object ( archetype *at );
extern char * artifact_msg ( int level, int booksize );
extern int baptize_altar ( object *op );
extern void bitstostring ( long bits, int num, char *str );
extern void blinded_sight ( object *op );
extern int blocked ( mapstruct *m, int x, int y );
extern int blocked_link ( object *ob, int x, int y );
extern int blocked_two ( object *op, int x, int y );
extern int blocks_cleric ( mapstruct *m, int x, int y );
extern int blocks_magic ( mapstruct *m, int x, int y );
extern int blocks_view ( mapstruct *m, int x, int y );
extern int book_overflow ( char *buf1, char *buf2, int booksize );
extern int buf_overflow ( char *buf1, char *buf2, int bufsize );
extern int calculate_difficulty ( mapstruct *m );
extern int can_pick ( object *who, object *item );
extern int change_abil ( object *op, object *tmp );
extern void change_attr_value ( living *stats, int attr, signed char value );
extern void change_book ( object *book, int msgtype );
extern void change_luck ( object *op, int value );
extern int change_map_light ( mapstruct *m, int change );
extern int check_altar_sacrifice ( object *altar, object *sacrifice );
extern int check_dm_add_exp_to_obj ( object *exp_ob, int i );
extern void check_formulae ( void );
extern void check_generators ( void );
extern void check_inv ( object *op, object *trig );
extern object * check_inv_recursive ( object *op, object *trig );
extern int check_path ( char *name, int prepend_dir );
extern void check_stat_bounds ( living *stats );
extern int check_trigger ( object *op, object *cause );
extern int check_walk_on ( object *op, object *originator );
extern void check_wall ( object *op, int x, int y );
extern void clean_object ( object *op );
extern char *clean_path ( char *file );
extern void clean_tmp_map ( mapstruct *m );
extern void clear_archetable ( void );
extern void clear_los ( object *op );
extern void clear_map ( mapstruct *m );
extern void clear_object ( object *op );
extern object *clone_arch ( int type );
extern void close_and_delete ( FILE *fp, int compressed );
extern void copy_map ( mapstruct *m1, mapstruct *m2 );
extern void copy_object ( object *op2, object *op );
extern void copy_owner ( object *op, object *clone );
extern int count_active ( void );
extern int count_free ( void );
extern int count_used ( void );
extern void create_all_treasures ( treasure *t, object *op, int flag, int difficulty, int tries );
extern void create_one_treasure ( treasurelist *tl, object *op, int flag, int difficulty, int tries );
extern char *create_pathname ( char *name );
extern object *create_singularity ( char *name );
extern void create_treasure ( treasurelist *t, object *op, int flag, int difficulty, int tries );
extern object *decrease_ob_nr ( object *op, int i );
extern void delete_map ( mapstruct *m );
extern char *describe_item ( object *op );
extern int dirdiff ( int dir1, int dir2 );
extern int distance ( object *ob1, object *ob2 );
extern void do_mood_floor ( object *op, object *op2 );
extern void drain_specific_stat ( object *op, int deplete_stats );
extern void drain_stat ( object *op );
extern void dummy_draw_info ( int a, int b, object *ob, const char *txt );
extern void dummy_function ( void );
extern void dummy_function_int ( int i );
extern void dummy_function_int_int ( int i, int j );
extern void dummy_function_int_ob_ob ( int n, object *ob, object *ob2 );
extern void dummy_function_map ( mapstruct *m );
extern void dummy_function_mapstr ( int a, mapstruct *map, char *str );
extern void dummy_function_ob ( object *ob );
extern void dummy_function_ob2 ( object *ob, object *ob2 );
extern int dummy_function_ob2int ( object *ob, object *ob2 );
extern void dummy_function_ob_int ( object *ob, int i );
extern void dummy_function_player_int ( player *p, int j );
extern void dummy_function_txtnr ( char *txt, int nr );
extern void dummy_move_apply_func ( object *ob, object *ob2, object *ob3 );
extern void dump_abilities ( void );
extern void dump_alchemy ( void );
extern void dump_alchemy_costs ( void );
extern void dump_all_archetypes ( void );
extern void dump_all_maps ( void );
extern void dump_all_objects ( void );
extern void dump_arch ( archetype *at );
extern void dump_artifacts ( void );
extern void dump_friendly_objects ( void );
extern void dump_gods ( void );
extern void dump_map ( mapstruct *m );
extern void dump_map_lights ( mapstruct *m );
extern void dump_monster_treasure ( char *name );
extern void dump_monster_treasure_rec ( char *name, treasure *t, int depth );
extern void dump_object ( object *op );
extern void dump_object2 ( object *op );
extern int enough_elapsed_time ( void );
extern void expand_lighted_sight ( object *op );
extern void expand_objects ( void );
extern void expand_sight ( object *op );
extern void fatal ( int err );
extern int FindFace ( char *name, int error );
extern int find_animation ( char *name );
extern archetype *find_archetype ( char *name );
extern artifactlist *find_artifactlist ( int type );
extern char find_color ( char *name );
extern int find_dir ( mapstruct *m, int x, int y, object *exclude );
extern int find_dir_2 ( int x, int y );
extern int find_first_free_spot ( archetype *at, mapstruct *m, int x, int y );
extern int find_free_spot ( archetype *at, mapstruct *m, int x, int y, int start, int stop );
extern long find_ingred_cost ( char *name );
extern object *find_object ( int i );
extern object *find_object_name ( char *str );
extern object *find_skill ( object *op, int skillnr );
extern char * find_string ( const char *str );
extern treasurelist *find_treasurelist ( char *name );
extern archetype *find_treasure_by_name ( treasure *t, char *name, int depth );
extern void first_arch_pass ( FILE *fp );
extern void fix_container ( object *container );
extern void fix_flesh_item ( object *item, object *donor );
extern void fix_generated_item ( object *op, object *creator, int difficulty, int max_magic );
extern void fix_player ( object *op );
extern char * formula_msg ( int level, int booksize );
extern void free_all_archs ( void );
extern void free_all_god ( void );
extern void free_all_images ( void );
extern void free_all_objects ( mapstruct *m );
extern void free_all_object_data ( void );
extern void free_all_readable ( void );
extern void free_all_recipes ( void );
extern void free_all_treasures ( void );
extern void free_artifact ( artifact *at );
extern void free_artifactlist ( artifactlist *al );
extern void free_charlinks ( linked_char *lc );
extern void free_map ( mapstruct *m, int flag );
extern void free_object ( object *ob );
extern void free_objectlink ( objectlink *ol );
extern void free_objectlinkpt ( oblinkpt *obp );
extern void free_player ( player *pl );
extern void free_string ( char *str );
extern void free_treasurestruct ( treasure *t );
extern void generate_artifact ( object *op, int difficulty );
extern object *generate_treasure ( treasurelist *t, int difficulty );
extern object *get_archetype ( char *name );
extern archetype *get_archetype_struct ( void );
extern signed char get_attr_value ( living *stats, int attr );
extern objectlink *get_button_links ( object *button );
extern int get_button_value ( object *button );
extern mapstruct *get_empty_map ( int sizex, int sizey );
extern recipelist * get_formulalist ( int i );
extern char *get_levelnumber ( int i );
extern mapstruct *get_linked_map ( char *path );
extern int get_magic ( int diff );
extern object *get_nearest_part ( object *op, object *pl );
extern object * get_next_mon ( object *tmp );
extern char *get_number ( int i );
extern object *get_object ( void );
extern objectlink *get_objectlink ( void );
extern oblinkpt *get_objectlinkpt ( void );
extern char *get_ob_diff ( object *op, object *op2 );
extern object *get_owner ( object *op );
extern player *get_player_ob ( void );
extern object * get_random_mon ( int level );
extern recipe * get_random_recipe ( recipelist *rpl );
extern recipelist * get_random_recipelist ( void );
extern godlink * get_rand_god ( void );
extern object *get_split_ob ( object *orig_ob, int nr );
extern void give_artifact_abilities ( object *op, object *artifct );
extern char * god_info_msg ( int level, int booksize );
extern unsigned long hasharch ( char *str, int tablesize );
extern int has_ability ( object *ob );
extern mapstruct *has_been_loaded ( char *name );
extern int has_carried_lights ( object *op );
extern void identify ( object *op );
extern char * ingred_name ( char *name );
extern void init_anim ( void );
extern void init_archetable ( void );
extern void init_archetypes ( void );
extern void init_archetype_pointers ( void );
extern void init_artifacts ( void );
extern void init_block ( void );
extern void init_defaults ( void );
extern void init_dynamic ( void );
extern void init_environ ( void );
extern void init_formulae ( void );
extern void init_function_pointers ( void );
extern void init_globals ( void );
extern void init_gods ( void );
extern void init_hash_table ( void );
extern void init_library ( void );
extern void init_objects ( void );
extern void init_readable ( void );
extern void init_vars ( void );
extern object *insert_ob_in_map ( object *op, mapstruct *m, object *originator );
extern void insert_ob_in_map_simple ( object *op, mapstruct *m );
extern object *insert_ob_in_ob ( object *op, object *where );
extern int isqrt ( int n );
extern int is_magical ( object *op );
extern object *is_player_inv ( object *op );
extern uint32 level_exp ( int level, double expmul );
extern int lex_load ( object *op );
extern int light_not_listed ( object *op );
extern void load_archetypes ( void );
extern int load_object ( FILE *fp, object *op, int bufstate );
extern void load_objects ( mapstruct *m, FILE *fp, int block );
extern mapstruct *load_original_map ( char *filename, int flags );
extern void load_treasures ( void );
extern artifact * locate_recipe_artifact ( recipe *rp );
extern void LOG ( LogLevel logLevel, char *format, ... );
extern void log_time ( long process_utime );
extern int look_up_spell_name ( char * spname );
extern char *ltostr10 ( signed long n );
extern int magic_from_difficulty ( int difficulty );
extern void make_path_to_file ( char *filename );
extern void make_sure_not_seen ( object *op );
extern void make_sure_seen ( object *op );
extern object * MapGetObjectZ ( mapstruct * emap, int x, int y, int z );
extern object * MapGetRealObject ( mapstruct * emap, int x, int y, int z );
extern int MapInsertObjectZ ( mapstruct *emap, object *o, int x, int y, int z );
extern void MapMoveScroll ( mapstruct *target, mapstruct *source, int dx, int dy );
extern mapstruct *MapMoveScrollResize ( mapstruct *source, int width, int height, int dx, int dy );
extern int MapObjectOut ( mapstruct *target, object *obj, int x, int y );
extern object *merge_ob ( object *op, object *top );
extern char * mon_desc ( object *mon );
extern char * mon_info_msg ( int level, int booksize );
extern void move_all_objects ( mapstruct *m1, mapstruct *m2 );
extern char * msgfile_msg ( int level, int booksize );
extern int need_identify ( object *op );
extern int new_exp ( object *ob );
extern int new_save_map ( mapstruct *m, int flag );
extern void no_maps_file ( char *filename );
extern int nstrtok ( char *buf1, char *buf2 );
extern int numb_ingred ( char *buf );
extern object *ObjectCreateArch ( archetype * at );
extern object *ObjectCreateClone ( object *asrc );
extern FILE *open_and_uncompress ( char *name, int flag, int *compressed );
extern int operate_altar ( object *altar, object **sacrifice );
extern void player_lvl_adj ( object *who, object *op );
extern object *pntr_to_god_obj ( godlink *godlnk );
extern object *present ( unsigned char type, mapstruct *m, int x, int y );
extern object *present_arch ( archetype *at, mapstruct *m, int x, int y );
extern object *present_arch_in_ob ( archetype *at, object *op );
extern object *present_in_ob ( unsigned char type, object *op );
extern void print_los ( object *op );
extern void print_monsters ( void );
extern void push_button ( object *op );
extern char *query_base_name ( object *op, int plural );
extern char *query_name ( object *op );
extern int query_refcount ( const char *str );
extern char *query_short_name ( object *op );
extern char *query_weight ( object *op );
extern int ReadBmapNames ( void );
extern mapstruct *ready_map_name ( char *name, int flags );
extern void refresh_map ( mapstruct *m );
extern void relink_objs_offset ( mapstruct *m1, mapstruct *m2, int dx, int dy );
extern void remove_button_link ( object *op );
extern void remove_carried_lights ( object *pl, mapstruct *map );
extern void remove_directory ( const char *path );
extern void remove_friendly_object ( object *op );
extern void remove_light_from_list ( object *light, object *op );
extern void remove_ob ( object *op );
extern void remove_statbonus ( object *op );
extern void reset_object ( object *op );
extern void reset_sleep ( void );
extern char * re_cmp ( char *str, char *regexp );
extern char *ring_desc ( object *op );
extern void save_double ( char *buf, char *name, double v );
extern void save_long ( char *buf, char *name, long n );
extern void save_object ( FILE *fp, object *op, int flag );
extern void save_objects ( mapstruct *m, FILE *fp, FILE *fp2 );
extern long seconds ( void );
extern void second_arch_pass ( FILE *fp );
extern void set_abs_magic ( object *op, int magic );
extern void set_attr_value ( living *stats, int attr, signed char value );
extern void set_block ( int x, int y, int bx, int by );
extern void set_cheat ( object *op );
extern void set_clean_tmp_files ( type_func_void addr );
extern void set_draw ( type_func_ob addr );
extern void set_draw_info ( type_func_int_int_ob_cchar addr );
extern void set_emergency_save ( type_func_int addr );
extern void set_esrv_del_item ( type_func_player_int addr );
extern void set_esrv_send_item ( type_func_ob_ob addr );
extern void set_esrv_update_item ( type_func_int_ob_ob addr );
extern void set_fix_auto_apply ( type_func_map addr );
extern void set_info_map ( type_func_int_map_char addr );
extern void set_init_blocksview_players ( type_func_void addr );
extern void set_magic ( int difficulty, object *op, int max_magic );
extern void set_map_reset_time ( mapstruct *map );
extern void set_max_time ( long t );
extern void set_monster_check_apply ( type_func_ob_ob addr );
extern void set_move_apply ( type_move_apply_func addr );
extern void set_move_creator ( type_func_ob addr );
extern void set_move_firewall ( type_func_ob addr );
extern void set_move_teleporter ( type_func_ob addr );
extern void set_owner ( object *op, object *owner );
extern void set_process_active_maps ( type_func_void addr );
extern void set_remove_friendly_object ( type_func_ob addr );
extern void set_ring_bonus ( object *op, int bonus );
extern void set_trap_adjust ( type_func_ob_int addr );
extern void set_update_buttons ( type_func_map addr );
extern int set_variable ( object *op, char *buf );
extern void set_wall ( object *op, int x, int y );
extern void sleep_delta ( void );
extern int special_potion ( object *op );
extern char * spellpath_msg ( int level, int booksize );
extern void ss_dump_statistics ( void );
extern char * ss_dump_table ( int what );
extern char *strdup_local ( char *str );
extern char *strerror_local ( int errnum );
extern int strtoint ( char *buf );
extern char * strtoktolin ( char *buf1, char *buf2 );
extern long strtol_local ( char *str, char **ptr, int base );
extern void sub_weight ( object *op, signed long weight );
extern signed long sum_weight ( object *op );
extern void tailor_readable_ob ( object *book, int msg_type );
extern char *tempnam_local ( char *dir, char *pfx );
extern void time_info ( object *op );
extern void trigger_move ( object *op, int state );
extern archetype *type_to_archetype ( int type );
extern void update_all_los ( mapstruct *map );
extern void update_button ( object *op );
extern void update_buttons ( mapstruct *m );
extern void update_los ( object *op );
extern void update_map_lights ( mapstruct *m );
extern void update_object ( object *op );
extern void update_ob_speed ( object *op );
extern object *update_position ( mapstruct *m, int x, int y );
extern void update_turn_face ( object *op );
extern void use_trigger ( object *op );
extern void verify_button_links ( mapstruct *map );
extern int wall ( mapstruct *m, int x, int y );
extern int was_destroyed ( object *op, tag_t old_tag );
extern void write_book_archive ( void );
extern int yyerror ( char *s );
extern void yyrestart ( FILE *input_file );
extern void yy_load_buffer_state ( void );

#endif /* __STDC__ */
#endif /* __CEXTRACT__ */
