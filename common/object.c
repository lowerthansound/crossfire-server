/*
 * static char *rcsid_object_c =
 *   "$Id$";
 */

/*
    CrossFire, A Multiplayer game for X-windows

    Copyright (C) 2000 Mark Wedel
    Copyright (C) 1992 Frank Tore Johansen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    The author can be reached via e-mail to mwedel@scruz.net
*/

/* Eneq(@csd.uu.se): Added weight-modifiers in environment of objects.
   sub/add_weight will transcend the environment updating the carrying
   variable. */

#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <global.h>
#include <object.h>
#include <funcpoint.h>
#include <skills.h> 

object objarray[STARTMAX]; /* All objects, allocated this way at first */
object *objects;           /* Pointer to the list of used objects */
object *free_objects;      /* Pointer to the list of unused objects */
object *active_objects;	/* List of active objects that need to be processed */

int nroffreeobjects = STARTMAX;  /* How many OBs allocated and free (free) */
int nrofallocobjects = STARTMAX; /* How many OBs allocated (free + used) */

int freearr_x[SIZEOFFREE]=
  {0,0,1,1,1,0,-1,-1,-1,0,1,2,2,2,2,2,1,0,-1,-2,-2,-2,-2,-2,-1,
   0,1,2,3,3,3,3,3,3,3,2,1,0,-1,-2,-3,-3,-3,-3,-3,-3,-3,-2,-1};
int freearr_y[SIZEOFFREE]=
  {0,-1,-1,0,1,1,1,0,-1,-2,-2,-2,-1,0,1,2,2,2,2,2,1,0,-1,-2,-2,
   -3,-3,-3,-3,-2,-1,0,1,2,3,3,3,3,3,3,3,2,1,0,-1,-2,-3,-3,-3};
int maxfree[SIZEOFFREE]=
  {0,9,10,13,14,17,18,21,22,25,26,27,30,31,32,33,36,37,39,39,42,43,44,45,
  48,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49};
int freedir[SIZEOFFREE]= {
  0,1,2,3,4,5,6,7,8,1,2,2,2,3,4,4,4,5,6,6,6,7,8,8,8,
  1,2,2,2,2,2,3,4,4,4,4,4,5,6,6,6,6,6,7,8,8,8,8,8};


/* Moved this out of define.h and in here, since this is the only file
 * it is used in.  Also, make it an inline function for cleaner
 * design.
 *
 * Function examines the 2 objects given to it, and returns true if
 * they can be merged together.
 *
 * Note that this function appears a lot longer than the macro it
 * replaces - this is mostly for clarity - a decent compiler should hopefully
 * reduce this to the same efficiency.
 *
 * Check nrof variable *before* calling CAN_MERGE()
 *
 * Improvements made with merge:  Better checking on potion, and also
 * check weight
 */

inline int CAN_MERGE(object *ob1, object *ob2) {

    /* A couple quicksanity checks */
    if ((ob1 == ob2) || (ob1->type != ob2->type)) return 0;

    if (ob1->speed != ob2->speed) return 0;
    /* Note sure why the following is the case - either the object has to
     * be animated or have a very low speed.  Is this an attempted monster
     * check?
     */
    if (!QUERY_FLAG(ob1,FLAG_ANIMATE) && FABS((ob1)->speed) > MIN_ACTIVE_SPEED)
	return 0;


    /* If the objects have been identified, set the BEEN_APPLIED flag.
     * This is to the comparison of the flags below will be OK.  We
     * just can't ignore the been applied or identified flags, as they
     * are not equal - just if it has been identified, the been_applied
     * flags lose any meaning.
     */
    if (QUERY_FLAG(ob1, FLAG_IDENTIFIED))
	SET_FLAG(ob1, FLAG_BEEN_APPLIED);

    if (QUERY_FLAG(ob2, FLAG_IDENTIFIED))
	SET_FLAG(ob2, FLAG_BEEN_APPLIED);


    /* the 0x400000 on flags2 is FLAG_INV_LOCK.  I don't think something
     * being locked in inventory should prevent merging.
     * 0x4 in flags3 is CLIENT_SENT 
     */
    if ((ob1->arch != ob2->arch) || 
	(ob1->stats.sp != ob2->stats.sp) ||
	(ob1->flags[0] != ob2->flags[0]) || 
	(ob1->flags[1] != ob2->flags[1]) ||
	((ob1->flags[2] & ~0x400000) != (ob2->flags[2] & ~ 0x400000)) ||
	((ob1->flags[3] & ~0x4) != (ob2->flags[3] & ~0x4)) || 
	(ob1->name != ob2->name) || 
	(ob1->title != ob2->title) ||
	(ob1->msg != ob2->msg) || 
	(ob1->weight != ob2->weight) ||
	(ob1->stats.food != ob2->stats.food) ||
	(memcmp(&ob1->resist, &ob2->resist, sizeof(ob1->resist))!=0) ||
	(ob1->attacktype != ob2->attacktype) ||
	(ob1->magic != ob2->magic) ||
	(ob1->slaying != ob2->slaying) ||
	(ob1->value != ob2->value) ||
	(ob1->animation_id != ob2->animation_id)
	) 
	    return 0;

    switch (ob1->type) {
	case SCROLL:
	    if (ob1->level != ob2->level) return 0;
	    break;

	case RING:
	    /* Don't merge applied rings - easier to keep them seperate, and
	     * it makes more sense (can easily unapply one ring).  Rings are
	     * the only objects that need this special code, as they are the
	     * only objects of the same type in which more than 1 can be
	     * applied at a time.
	     *
	     * Note - there is no break so we fall into the POTION/AMULET
	     * check below.
	     */
	    if (QUERY_FLAG(ob1, FLAG_APPLIED) || QUERY_FLAG(ob2, FLAG_APPLIED))
		return 0;

	case POTION:
	case AMULET:
	    /* This should compare the value of the stats, and not the pointer
	     * itself.  There can be cases were potions seem to loose their
	     * plus
	     */
	    if (memcmp(&ob1->stats,&ob2->stats, sizeof(living))) return 0;
	    break;
    }
    /* Everything passes, must be OK. */
    return 1;
}

/*
 * sum_weight() is a recursive function which calculates the weight
 * an object is carrying.  It goes through in figures out how much
 * containers are carrying, and sums it up.
 */
signed long sum_weight(object *op) {
  signed long sum;
  object *inv;
  for(sum = 0, inv = op->inv; inv != NULL; inv = inv->below) {
    if (inv->inv)
	sum_weight(inv);
    sum += inv->carrying + (inv->nrof ? inv->weight * inv->nrof : inv->weight);
  }
  if (op->type == CONTAINER && op->stats.Str)
    sum = (sum * (100 - op->stats.Str))/100;
  if(op->carrying != sum)
    op->carrying = sum;
  return sum;
}


/*
 * Eneq(@csd.uu.se): Since we can have items buried in a character we need
 * a better check.  We basically keeping traversing up until we can't
 * or find a player.
 */

object *is_player_inv (object *op) { 
    for (;op!=NULL&&op->type!=PLAYER; op=op->env)
      if (op->env==op)
	op->env = NULL;
    return op;
}

/*
 * Used by: Crossedit: dump. Server DM commands: dumpbelow, dump.
 *	Some error messages.
 * The result of the dump is stored in the static global errmsg array.
 */

void dump_object2(object *op) {
  char *cp;
/*  object *tmp;*/

  if(op->arch!=NULL) {
      strcat(errmsg,"arch ");
      strcat(errmsg,op->arch->name?op->arch->name:"(null)");
      strcat(errmsg,"\n");
      if((cp=get_ob_diff(op,&empty_archetype->clone))!=NULL)
	strcat(errmsg,cp);
#if 0
      /* Don't dump player diffs - they are too long, mostly meaningless, and
       * will overflow the buffer.
       * Changed so that we don't dump inventory either.  This may
       * also overflow the buffer.
       */
      if(op->type!=PLAYER && (cp=get_ob_diff(op,&empty_archetype->clone))!=NULL)
        strcat(errmsg,cp);
      for (tmp=op->inv; tmp; tmp=tmp->below)
        dump_object2(tmp);
#endif
      strcat(errmsg,"end\n");
  } else {
      strcat(errmsg,"Object ");
      if (op->name==NULL) strcat(errmsg, "(null)");
      else strcat(errmsg,op->name);
      strcat(errmsg,"\n");
#if 0
      if((cp=get_ob_diff(op,&empty_archetype->clone))!=NULL)
        strcat(errmsg,cp);
      for (tmp=op->inv; tmp; tmp=tmp->below)
        dump_object2(tmp);
#endif
      strcat(errmsg,"end\n");
  }
}

/*
 * Dumps an object.  Returns output in the static global errmsg array.
 */

void dump_object(object *op) {
  if(op==NULL) {
    strcpy(errmsg,"[NULL pointer]");
    return;
  }
  errmsg[0]='\0';
  dump_object2(op);
}

/*
 * This is really verbose...Can be triggered by the P key while in DM mode.
 * All objects are dumped to stderr (or alternate logfile, if in server-mode)
 */

void dump_all_objects() {
  object *op;
  for(op=objects;op!=NULL;op=op->next) {
    dump_object(op);
    fprintf(logfile, "Object %d\n:%s\n", op->count, errmsg);
  }
}

/*
 * get_nearest_part(multi-object, object 2) returns the part of the
 * multi-object 1 which is closest to the second object.
 * If it's not a multi-object, it is returned.
 */

object *get_nearest_part(object *op,object *pl) {
  object *tmp,*closest;
  int last_dist,i;
  if(op->more==NULL)
    return op;
  for(last_dist=distance(op,pl),closest=op,tmp=op->more;tmp!=NULL;tmp=tmp->more)
    if((i=distance(tmp,pl))<last_dist)
      closest=tmp,last_dist=i;
  return closest;
}

/*
 * Returns the object which has the count-variable equal to the argument.
 */

object *find_object(int i) {
  object *op;
  for(op=objects;op!=NULL;op=op->next)
    if(op->count==i)
      break;
 return op;
}

/*
 * Returns the first object which has a name equal to the argument.
 * Used only by the patch command, but not all that useful.
 * Enables features like "patch <name-of-other-player> food 999"
 */

object *find_object_name(char *str) {
  char *name=add_string(str);
  object *op;
  for(op=objects;op!=NULL;op=op->next)
    if(op->name==name)
      break;
  free_string(name);
  return op;
}

void free_all_object_data() {
    object *op, *next;

    for (op=objects; op!=NULL; op=next) {
	next=op->next;
	if (!op->head && !QUERY_FLAG(op,FLAG_REMOVED))
		remove_ob(op);
	if (!op->head && !QUERY_FLAG(op,FLAG_FREED))
		free_object(op);
    }
#if 0
    /* In theory, we should do this.  In practice, it is fairly difficult
     * because objects are created in groups of 100, and there is no record
     * of those starting pointers.
     */
    for (op=free_objects; op!=NULL; ) {
	next=op->next;
	free(op);
	nrofallocobjects--;
	nroffreeobjects--;
	last=op;
	op=next;
    }
#endif
    LOG(llevDebug,"%d allocated objects, %d free objects, STARMAX=%d\n", 
	nrofallocobjects, nroffreeobjects,STARTMAX);
}

/*
 * Returns the object which this object marks as being the owner.
 * A id-scheme is used to avoid pointing to objects which have been
 * freed and are now reused.  If this is detected, the owner is
 * set to NULL, and NULL is returned.
 * (This scheme should be changed to a refcount scheme in the future)
 */

object *get_owner(object *op) {
  if(op->owner==NULL)
    return NULL;
  if(!QUERY_FLAG(op->owner,FLAG_FREED) && op->owner->count==op->ownercount)
    return op->owner;
  op->owner=NULL,op->ownercount=0;
  return NULL;
}

/*
 * Sets the owner of the first object to the second object.
 * Also checkpoints a backup id-scheme which detects freeing (and reusage)
 * of the owner object.
 * See also get_owner()
 */

static void set_owner_simple (object *op, object *owner)
{
    /* next line added to allow objects which own objects */ 
    /* Add a check for ownercounts in here, as I got into an endless loop
     * with the fireball owning a poison cloud which then owned the
     * fireball.  I believe that was caused by one of the objects getting
     * freed and then another object replacing it.  Since the ownercounts
     * didn't match, this check is valid and I believe that cause is valid.
     */
    while (owner->owner && owner!=owner->owner && 
	owner->ownercount==owner->owner->count) owner=owner->owner;

    /* IF the owner still has an owner, we did not resolve to a final owner.
     * so lets not add to that.
     */
    if (owner->owner) return;

    op->owner=owner;

    op->ownercount=owner->count;
    owner->refcount++;
}

#ifdef ALLOW_SKILLS
static void set_skill_pointers (object *op, object *chosen_skill,
	object *exp_obj)
{
    op->chosen_skill = chosen_skill;
    op->exp_obj = exp_obj;

    /* unfortunately, we can't allow summoned monsters skill use
     * because we will need the chosen_skill field to pick the
     * right skill/stat modifiers for calc_skill_exp(). See
     * hit_player() in server/attack.c -b.t.
     */
    CLEAR_FLAG (op, FLAG_CAN_USE_SKILL);
    CLEAR_FLAG (op, FLAG_READY_SKILL);
}
#endif

/*
 * Sets the owner and sets the skill and exp pointers to owner's current
 * skill and experience objects.
 */
void set_owner (object *op, object *owner)
{
    if(owner==NULL||op==NULL)
	return;
    set_owner_simple (op, owner);

#ifdef ALLOW_SKILLS /* set the pointers in op to inherit owners skill, exp_obj */ 
    if (owner->type == PLAYER && owner->chosen_skill)
        set_skill_pointers (op, owner->chosen_skill,
                            owner->chosen_skill->exp_obj);
    else if (op->type != PLAYER)
	CLEAR_FLAG (op, FLAG_READY_SKILL);
#endif
}

/* Set the owner to clone's current owner and set the skill and experience
 * objects to clone's objects (typically those objects that where the owner's
 * current skill and experience objects at the time when clone's owner was
 * set - not the owner's current skill and experience objects).
 *
 * Use this function if player created an object (e.g. fire bullet, swarm
 * spell), and this object creates further objects whose kills should be
 * accounted for the player's original skill, even if player has changed
 * skills meanwhile.
 */
void copy_owner (object *op, object *clone)
{
    object *owner = get_owner (clone);
    if (owner == NULL)
      return;
    set_owner_simple (op, owner);

#ifdef ALLOW_SKILLS
    if (clone->chosen_skill)
        set_skill_pointers (op, clone->chosen_skill, clone->exp_obj);
    else if (op->type != PLAYER)
	CLEAR_FLAG (op, FLAG_READY_SKILL);
#endif
}

/*
 * Resets vital variables in an object
 */

void reset_object(object *op) {
  op->name = NULL;
  op->title = NULL;
  op->race = NULL;
  op->slaying = NULL;
  op->msg = NULL;
  clear_object(op);
}
/*
 * clear_object() frees everything allocated by an object, and also
 * clears all variables and flags to default settings.
 */

void clear_object(object *op) {
   int tmp;

  if(op->name!=NULL) {
    free_string(op->name);
    op->name=NULL;
  }
  if(op->title != NULL) {
    free_string(op->title);
    op->title = NULL;
  }
  if(op->race!=NULL) {
    free_string(op->race);
    op->race=NULL;
  }
  if(op->slaying!=NULL) {
    free_string(op->slaying);
    op->slaying=NULL;
  }
  if(op->msg!=NULL) {
    free_string(op->msg);
    op->msg=NULL;
  }
  for (tmp=0; tmp<=(NUM_FLAGS/32); tmp++)
	op->flags[tmp] = 0;
  /* This is more or less true */
  SET_FLAG(op, FLAG_REMOVED);
  op->map=NULL;
  op->below=NULL;
  op->above=NULL;
  op->owner=NULL;
  op->refcount=0;
  op->inv=NULL;
  op->env=NULL;
  op->container=NULL;
  op->more=NULL;
  op->head=NULL;
  op->arch=NULL;
  op->other_arch=NULL;
  op->enemy=NULL;
  op->contr = NULL;
  op->sk_list = NULL;
  op->weight=0,op->carrying=0,op->weight_limit=0;
  op->anim_speed=0;
  op->level=0;
  op->value=op->run_away=0;
  op->invisible=0;
  op->last_heal=0,op->last_sp=0,op->last_grace=0,op->last_eat=0;
  op->nrof=0;
  op->ownercount=0;
  op->attacktype=0;
  memset(&op->resist, 0, sizeof(op->resist));
  op->stats.exp=0;
  op->x=0; op->y=0; op->ox=0; op->oy=0;
  op->stats.dam=0,op->stats.wc=0,op->stats.ac=0;
  op->stats.luck=0;
  op->stats.food=0;
  op->stats.sp=op->stats.maxsp=op->stats.hp=op->stats.maxhp=0;
  op->stats.grace=op->stats.maxgrace=0;
  op->expmul=1.0;
  op->direction=0;
  op->glow_radius=0;
  op->stats.Str=op->stats.Dex=op->stats.Con=0;
  op->stats.Wis=op->stats.Cha=op->stats.Int=op->stats.Pow=0;
  op->material=op->magic=op->state=op->type=0;
  op->face = blank_face;

  /* The object should already have been removed from the speed list
   * before this function is called
   */
  op->speed=op->speed_left=0;
  op->pick_up=0;
  op->can_apply=0;
  op->will_apply=0;
  op->move_status = 0;
  op->move_type = 0;
  op->path_repelled = 0;
  op->path_attuned = 0;
  op->path_denied = 0;
#ifdef CASTING_TIME
  op->casting = -1;
  op->spelltype = 0;
  op->spell = NULL;
#endif
  op->randomitems=NULL;
  op->spellitem = NULL;
  op->animation_id=0;

}

/*
 * copy object first frees everything allocated by the second object,
 * and then copies the contends of the first object into the second
 * object, allocating what needs to be allocated.
 */

void copy_object(object *op2, object *op) {
  int is_freed=QUERY_FLAG(op,FLAG_FREED),is_removed=QUERY_FLAG(op,FLAG_REMOVED);

  if(op->name!=NULL)
    free_string(op->name);
  if(op->title!=NULL)
    free_string(op->title);
  if(op->race!=NULL)
    free_string(op->race);
  if(op->slaying!=NULL)
    free_string(op->slaying);
  if(op->msg!=NULL)
    free_string(op->msg);
  (void) memcpy((void *)((char *) op +offsetof(object,name)),
                (void *)((char *) op2+offsetof(object,name)),
                sizeof(object)-offsetof(object, name));
  if(is_freed)
    SET_FLAG(op,FLAG_FREED);
  if(is_removed)
    SET_FLAG(op,FLAG_REMOVED);
  if(op->name!=NULL)
    add_refcount(op->name);
  if(op->title!=NULL)
    add_refcount(op->title);
  if(op->race!=NULL)
    add_refcount(op->race);
  if(op->slaying!=NULL)
    add_refcount(op->slaying);
  if(op->msg!=NULL)
    add_refcount(op->msg);
  if((op2->speed<0) && !editor)
    op->speed_left=op2->speed_left-RANDOM()%200/100.0;
  update_ob_speed(op);
}

/*
 * expand_objects() allocates more objects for the list of unused objects.
 * It is called from get_object() if the unused list is empty.
 */

void expand_objects() {
  int i;
  object *new;
  new = (object *) CALLOC(OBJ_EXPAND,sizeof(object));

  if(new==NULL)
    fatal(OUT_OF_MEMORY);
  free_objects=new;
  new[0].prev=NULL;
  new[0].next= &new[1],
  SET_FLAG(&(new[0]), FLAG_REMOVED);
  SET_FLAG(&(new[0]), FLAG_FREED);

  for(i=1;i<OBJ_EXPAND-1;i++) {
    new[i].next= &new[i+1],
    new[i].prev= &new[i-1],
    SET_FLAG(&(new[i]), FLAG_REMOVED);
    SET_FLAG(&(new[i]), FLAG_FREED);
  }
  new[OBJ_EXPAND-1].prev= &new[OBJ_EXPAND-2],
  new[OBJ_EXPAND-1].next=NULL,
  SET_FLAG(&(new[OBJ_EXPAND-1]), FLAG_REMOVED);
  SET_FLAG(&(new[OBJ_EXPAND-1]), FLAG_FREED);

  nrofallocobjects += OBJ_EXPAND;
  nroffreeobjects += OBJ_EXPAND;
}

/*
 * get_object() grabs an object from the list of unused objects, makes
 * sure it is initialised, and returns it.
 * If there are no free objects, expand_objects() is called to get more.
 */

object *get_object() {
  object *op;
  if(free_objects==NULL) {
    expand_objects();
  }
  op=free_objects;
  if(!QUERY_FLAG(op,FLAG_FREED)) {
    LOG(llevError,"Fatal: Getting busy object.\n");
  }
  free_objects=op->next;
  if(free_objects!=NULL)
    free_objects->prev=NULL;
  op->count= ++ob_count;
  op->name=NULL;
  op->title=NULL;
  op->race=NULL;
  op->slaying=NULL;
  op->msg=NULL;
  op->next=objects;
  op->prev=NULL;
  op->active_next = NULL;
  op->active_prev = NULL;
  if(objects!=NULL)
    objects->prev=op;
  objects=op;
  clear_object(op);
  SET_FLAG(op,FLAG_REMOVED);
  nroffreeobjects--;
  return op;
}

/*
 * If an object with the IS_TURNABLE() flag needs to be turned due
 * to the closest player being on the other side, this function can
 * be called to update the face variable, _and_ how it looks on the map.
 */

void update_turn_face(object *op) {
    if(!QUERY_FLAG(op,FLAG_IS_TURNABLE)||op->arch==NULL)
	return;
    SET_ANIMATION(op, op->direction);
    update_object(op);
}

/*
 * Updates the speed of an object.  If the speed changes from 0 to another
 * value, or vice versa, then add/remove the object from the active list.
 * This function needs to be called whenever the speed of an object changes.
 */

void update_ob_speed(object *op) {
    extern int arch_init;

    /* No reason putting the archetypes objects on the speed list,
     * since they never really need to be updated.
     */

    if (QUERY_FLAG(op, FLAG_FREED) && op->speed) {
	LOG(llevError,"Object %s is freed but has speed.\n", op->name);
#ifdef MANY_CORES
	abort();
#endif
	op->speed = 0;
    }
    if (arch_init) {
	return;
    }
    if (FABS(op->speed)>MIN_ACTIVE_SPEED) {
	/* If already on active list, don't do anything */
	if (op->active_next || op->active_prev || op==active_objects)
	    return;

        /* process_events() expects us to insert the object at the beginning
         * of the list. */
	op->active_next = active_objects;
	if (op->active_next!=NULL)
		op->active_next->active_prev = op;
	active_objects = op;
    }
    else {
	/* If not on the active list, nothing needs to be done */
	if (!op->active_next && !op->active_prev && op!=active_objects)
	    return;

	if (op->active_prev==NULL) {
	    active_objects = op->active_next;
	    if (op->active_next!=NULL)
		op->active_next->active_prev = NULL;
	}
	else {
	    op->active_prev->active_next = op->active_next;
	    if (op->active_next)
		op->active_next->active_prev = op->active_prev;
	}
	op->active_next = NULL;
	op->active_prev = NULL;
    }
}


/*
 * update_object() updates the array which represents the map.
 * It takes into account invisible objects (and represent squares covered
 * by invisible objects by whatever is below them (unless it's another
 * invisible object, etc...)
 * If the object being updated is beneath a player, the look-window
 * of that player is updated (this might be a suboptimal way of
 * updating that window, though, since update_object() is called _often_)
 */

void update_object(object *op) {
    object *tmp, *player;

    if(op->env!=NULL) {
	tmp=is_player_inv(op->env);
	/* If we are in ericserver mode, I think we should send an update
	 * or something.  Ideally, client should handle animations on its
	 * own.
	 */
	return;
    }

    /* Can be null if the player has quit but window still exists. */
    if (op->map->map != NULL) {
	player=update_position (op->map, op->x, op->y);
	/*
	 * insert_ob_in_map calls update_ob.  insert_ob_in_map is called with
	 * the player object anytime he moves - as such, we compare the object
	 * being updated against the player object so that we don't send
	 * unnecessary update commands.  Also, the client doesn't do anything
	 * with the players face anyways.
	 * As a note - we assume that we get here because the face has changed -
	 * this might not always be the case, but seems to be a good amount of
	 * the time, and its easier to do this than track down all the places in
	 * the code that changes the face.
	 *
	 * Add a check so that if the item that is changing is hidden from
	 * the player (ie, below the floor), we don't send an update to the
	 * player.
	 * If update_look has been set, that is going to send all the stuff
	 * anyways, so no reason to send an update for it
	 */

	if (player && player!=op && !player->contr->socket.update_look) {
	    for (tmp=op; tmp!=NULL; tmp=tmp->above)
		if (QUERY_FLAG(tmp,FLAG_IS_FLOOR) && !tmp->invisible) break;

	    if (!tmp) esrv_update_item_func(UPD_FACE, player, op);
	}
    }

    if(op->more!=NULL)
	update_object(op->more);
}


/* This updates how the map looks at a specified space.  We use
 * some time saving techniques instead of space - each map has 2 or
 * 3 arrays - one for floors, one for top object, and one for an
 * additional object (if DOUBLE_FLOOR patch).  Changed things around
 * so that we now have a visibility flag, and if that is set, then the
 * highest object with that visibility gets put into the addition object
 * slot.
 *
 * Return player object ifwe find one.
 * update_object (which calls this) needs to know if it should send an 
 * update to the client.  Update object could check for that, but
 * since this looks through the space anyways, might as well this function
 * return it for us.
 */
object *update_position (mapstruct *m, int x, int y) {
    object *tmp, *last = NULL, *player=NULL;
    unsigned int flags = 0;
    MapLook top,floor, f1;


    f1=blank_look;
    top=blank_look;
    floor=blank_look;

    for (tmp = get_map_ob (m, x, y); tmp; last = tmp, tmp = tmp->above) {

	/* This call is needed in order to update objects the player
	 * is standign in that have animations (ie, grass, fire, etc).
	 * However, it also causes the look window to be re-drawn
	 * 3 times each time the player moves, because many of the
	 * functions the move_player calls eventualy call this.
	 *
	 * Always put the player down for drawing.
	 */
	if (tmp->type==PLAYER && !tmp->invisible) {
	    top.face = tmp->face;
	    player=tmp;
	}
	else if (QUERY_FLAG(tmp,FLAG_IS_FLOOR)) {
	    f1=blank_look;
	    if (!tmp->invisible)
		floor.face = tmp->face;
	}
	/* Find the highest visible face around */
	if (tmp->face->visibility > f1.face->visibility && !tmp->invisible) 
	    f1.face = tmp->face;

	if (tmp==tmp->above) {
	    LOG(llevError, "Error in structure of map\n");
	    exit (-1);
	}
      
	if (QUERY_FLAG(tmp,FLAG_ALIVE))
	    flags |= P_IS_ALIVE;
	if (QUERY_FLAG(tmp,FLAG_NO_PASS))
	    flags |= P_NO_PASS;
	if (QUERY_FLAG(tmp,FLAG_NO_MAGIC))
	    flags |= P_NO_MAGIC;
	if (QUERY_FLAG(tmp,FLAG_DAMNED))
	    flags |= P_NO_CLERIC;
	if (QUERY_FLAG(tmp,FLAG_PASS_THRU))
	    flags |= P_PASS_THRU;
	if (QUERY_FLAG(tmp,FLAG_BLOCKSVIEW))
	    flags |= P_BLOCKSVIEW;

    } /* for stack of objects */

    top.flags = flags;

    /* At this point, we have a floor.face (if there is a floor),
     * and the floor is set - we are not going to touch it at
     * this point.
     * f1 contains the highest visibility face. 
     * top contains a player face, if there is one.
     *
     * We now need to fill in top.face and/or f1.face.
     */

    /* If the top face also happens to be high visibility, re-do our
     * middle face.  This only happens when top.face is a player.
     */
    if (top.face == f1.face) f1.face=blank_look.face;

    /* There are three posibilities at this point:
     * 1) top face is set, need f1 to be set.
     * 2) f1 is set, need to set top.
     * 3) neither f1 or top is set - need to set both.
     */

    for (tmp=last; tmp; tmp=tmp->below) {
	/* Once we get to a floor, stop, since we already have a floor object */
	if (QUERY_FLAG(tmp,FLAG_IS_FLOOR)) break;

	/* If two top faces are already set, quit processing */
	if ((top.face != blank_look.face) && (f1.face != blank_look.face)) break;

	/* Only show visible faces, unless its the editor - show all */
	if (!tmp->invisible || editor) {
	    /* Fill in top if needed */
	    if (top.face == blank_look.face) {
		top.face = tmp->face;
	    } else {
		/* top is already set - we should only get here if
		 * f1 is not set
		 *
		* Set the middle face and break out, since there is nothing
		 * more to fill in.  We don't check visiblity here, since
		 * 
		 */
		if (tmp->face  != top.face ) {
		    f1.face = tmp->face;
		    break;
		}
	    }
	}
    }
    set_map (m, x, y, &top);
    set_map_floor2(m, x, y, &f1);
    set_map_floor(m,x,y,&floor);

    return player;
}

/*
 * free_object() frees everything allocated by an object, removes
 * it from the list of used objects, and puts it on the list of
 * free objects.  The IS_FREED() flag is set in the object.
 * The object must have been removed by remove_ob() first for
 * this function to succeed.
 */

void free_object(object *ob) {
  object *tmp,*op;

    if (!QUERY_FLAG(ob,FLAG_REMOVED)) {
	LOG(llevDebug,"Free object called with non removed object\n");
	dump_object(ob);
#ifdef MANY_CORES
	abort();
#endif
    }
  if(QUERY_FLAG(ob,FLAG_FRIENDLY)) {
    LOG(llevMonster,"Warning: tried to free friendly object.\n");
    remove_friendly_object(ob);
  }
  if(QUERY_FLAG(ob,FLAG_FREED)) {
    dump_object(ob);
    LOG(llevError,"Trying to free freed object.\n%s\n",errmsg);
    return;
  }
  if(ob->more!=NULL) {
    free_object(ob->more);
    ob->more=NULL;
  }
  if (ob->inv) {
    if (ob->map==NULL || ob->map->in_memory!=MAP_IN_MEMORY ||
       wall(ob->map,ob->x,ob->y))
    {
      op=ob->inv;
      while(op!=NULL) {
        tmp=op->below;
        remove_ob(op);
        free_object(op);
        op=tmp;
      }
    }
    else {
      op=ob->inv;
      while(op!=NULL) {
        tmp=op->below;
        remove_ob(op);
        if(QUERY_FLAG(op,FLAG_STARTEQUIP)||QUERY_FLAG(op,FLAG_NO_DROP) ||
	  op->type==RUNE)
          free_object(op);
        else {
          op->x=ob->x,op->y=ob->y;
          insert_ob_in_map(op,ob->map,NULL); /* Insert in same map as the envir */
        }
        op=tmp;
      }
    }
  }
  /* Remove object from the active list */
  ob->speed = 0;
  update_ob_speed(ob);

  SET_FLAG(ob, FLAG_FREED);
  ob->count = 0;
  /* First free the object from the used objects list: */
  if(ob->prev==NULL) {
    objects=ob->next;
    if(objects!=NULL)
      objects->prev=NULL;
  }
  else {
    ob->prev->next=ob->next;
    if(ob->next!=NULL)
      ob->next->prev=ob->prev;
  }
  
  /* Now link it with the free_objects list: */
  ob->prev=NULL;
  ob->next=free_objects;
  if(free_objects!=NULL)
    free_objects->prev=ob;
  free_objects=ob;
  if(ob->name!=NULL) {
    free_string(ob->name);
    ob->name=NULL;
  }
  if(ob->title!=NULL) {
    free_string(ob->title);
    ob->title=NULL;
  }
  if(ob->race!=NULL) {
    free_string(ob->race);
    ob->race=NULL;
  }
  if(ob->slaying!=NULL) {
    free_string(ob->slaying);
    ob->slaying=NULL;
  }
  if(ob->msg!=NULL) {
    free_string(ob->msg);
    ob->msg=NULL;
  }
  nroffreeobjects++;
}

/*
 * count_free() returns the number of objects on the list of free objects.
 */

int count_free() {
  int i=0;
  object *tmp=free_objects;
  while(tmp!=NULL)
    tmp=tmp->next, i++;
  return i;
}

/*
 * count_used() returns the number of objects on the list of used objects.
 */

int count_used() {
  int i=0;
  object *tmp=objects;
  while(tmp!=NULL)
    tmp=tmp->next, i++;
  return i;
}

/*
 * count_active() returns the number of objects on the list of active objects.
 */

int count_active() {
  int i=0;
  object *tmp=active_objects;
  while(tmp!=NULL)
    tmp=tmp->active_next, i++;
  return i;
}

/*
 * sub_weight() recursively (outwards) subtracts a number from the
 * weight of an object (and what is carried by it's environment(s)).
 */

void sub_weight (object *op, signed long weight) {
  while (op != NULL) {
    if (op->type == CONTAINER) {
      weight=(signed long)(weight*(100-op->stats.Str)/100);
    }
    op->carrying-=weight;
    op = op->env;
  }
}

/* remove_ob(op):
 *   This function removes the object op from the linked list of objects
 *   which it is currently tied to.  When this function is done, the
 *   object will have no environment.  If the object previously had an
 *   environment, the x and y coordinates will be updated to
 *   the previous environment.
 *   Beware: This function is called from the editor as well!
 */

void remove_ob(object *op) {
  object *tmp,*last=NULL;
  object *otmp;
  tag_t tag;
  int check_walk_off;

  if(QUERY_FLAG(op,FLAG_REMOVED)) {
    dump_object(op);
    LOG(llevError,"Trying to remove removed object.\n%s\n",errmsg);

/* Might as well dump core here.  In most cases, the function calling
 * remove_ob makes assumptions about success, and the validity of the
 * next, above, and below pointers, and a core dump would probably result
 * shortly anyways.
 */
#ifdef MANY_CORES
    abort();
#else
    if(op->map!=NULL && op->map->in_memory == MAP_IN_MEMORY) {
      op->map->need_refresh=1;
      LOG(llevDebug,"Marked map %s for refresh.\n",op->map->path);
    }
#endif
    if(get_map_ob(op->map,op->x,op->y)==op)
    { /* we are at bottom [Smart! -Frank] */
       if (op->below)
	  set_map_ob(op->map,op->x,op->y,op->below);
       else if (op->above)
	  set_map_ob(op->map,op->x,op->y,op->above);
       else 
	  set_map_ob(op->map,op->x,op->y,NULL);
    }
    return; /* Why did you comment this away at DoCS?? It just makes it worse */
  }
  if(op->more!=NULL)
    remove_ob(op->more);
  SET_FLAG(op, FLAG_REMOVED);
  if(op->env!=NULL) {
    if(op->nrof)
      sub_weight(op->env, op->weight*op->nrof);
    else
      sub_weight(op->env, op->weight+op->carrying);
    if ((otmp=is_player_inv(op->env))!=NULL && otmp->contr && 
	!QUERY_FLAG(otmp,FLAG_NO_FIX_PLAYER))
      fix_player(otmp);
    if(op->above!=NULL)
      op->above->below=op->below;
    else
      op->env->inv=op->below;
    if(op->below!=NULL)
      op->below->above=op->above;
    op->x=op->env->x,op->y=op->env->y;
    op->ox=op->x,op->oy=op->y;
    op->map=op->env->map;
    op->above=NULL,op->below=NULL;
    op->env=NULL;
    return;
  }
  if (op->map == NULL) return;
  if(out_of_map(op->map,op->x,op->y))
    op->x=0,op->y=0,op->ox=0,op->oy=0;
  if(op->above!=NULL) {                /* Don't change map, we're not on top */
    op->above->below=op->below;        /* Link above with below */
    if(op->below!=NULL) {              /* Something below us? */
      op->below->above=op->above;      /* Yes, link above with below */
      op->below=NULL;
    }
    else {                             /* Nothing below, tell the floor what */
      if(get_map_ob(op->map,op->x,op->y)!=op) {
        dump_object(op);
        LOG(llevError,"Object squizez another object:\n%s\n",errmsg);
        dump_object(get_map_ob(op->map,op->x,op->y));
        LOG(llevError,"%s\n",errmsg);
      }
      set_map_ob(op->map,op->x,op->y,op->above);  /* goes on above it. */
    }
    op->above=NULL;                      
  }
  else if(op->below!=NULL) {
    op->below->above=NULL,             /* It is now on top */
    op->below=NULL;
  } else {
      if(get_map_ob(op->map,op->x,op->y)!=op) {
        LOG(llevError,"Object squizez another object:\n");
        dump_object(op);
        LOG(llevError,"%s\n",errmsg);
        dump_object(get_map_ob(op->map,op->x,op->y));
        LOG(llevError,"%s\n",errmsg);
      }
    set_map_ob(op->map,op->x,op->y,NULL);           /* No more objects here */
  }
  if (op->map->in_memory == MAP_SAVING)
    return;
  tag = op->count;
  check_walk_off = ! QUERY_FLAG (op, FLAG_NO_APPLY);
  for(tmp=get_map_ob(op->map,op->x,op->y);tmp!=NULL;tmp=tmp->above) {
    /* No point updating the players look faces if he is the object
     * being removed.
     */
    if(tmp->type==PLAYER && tmp!=op) {
	/* If a container that the player is currently using somehow gets
	 * removed (most likely destroyed), update the player view
	 * appropriately.
	 */
	if (tmp->container==op) {
	    CLEAR_FLAG(op, FLAG_APPLIED);
	    tmp->container=NULL;
	}
	tmp->contr->socket.update_look=1;
    }
    if (check_walk_off && (QUERY_FLAG (op, FLAG_FLYING) ?
        QUERY_FLAG (tmp, FLAG_FLY_OFF) : QUERY_FLAG (tmp, FLAG_WALK_OFF)))
    {
        move_apply_func (tmp, op, NULL);
        if (was_destroyed (op, tag)) {
          LOG (llevError, "BUG: remove_ob(): name %s, archname %s destroyed "
               "leaving object\n", tmp->name, tmp->arch->name);
        }
    }

    /* Eneq(@csd.uu.se): Fixed this to skip tmp->above=tmp */

    if(tmp->above == tmp)
      tmp->above = NULL;
    last=tmp;
  }
  if (last==NULL)
    update_position(op->map, op->x, op->y);
  else
    update_object(last);

  if(QUERY_FLAG(op,FLAG_BLOCKSVIEW)	/* Should be harmless from editor, */
#ifdef USE_LIGHTING
     ||(op->glow_radius>0) 
#endif
  ) 
    update_all_los(op->map);	/* first_player is no longer set there */

}

/*
 * merge_ob(op,top):
 *
 * This function goes through all objects below and including top, and
 * merges op to the first matching object.
 * If top is NULL, it is calculated.
 * Returns pointer to object if it succeded in the merge, otherwise NULL
 */

object *merge_ob(object *op, object *top) {
  if(!op->nrof)
    return 0;
  if(top==NULL)
    for(top=op;top!=NULL&&top->above!=NULL;top=top->above);
  for(;top!=NULL;top=top->below) {
    if(top==op)
      continue;
    if (CAN_MERGE(op,top))
    {
      top->nrof+=op->nrof;
/*      CLEAR_FLAG(top,FLAG_STARTEQUIP);*/
      op->weight = 0; /* Don't want any adjustements now */
      remove_ob(op);
      free_object(op);
      return top;
    }
  }
  return NULL;
}


/* This is a simple version of the insert_ob_in_map function below. 
 * We do stuff similar to insert_ob_in_map, but don't merge objects
 * or check apply.  This is needed by at least the polymorph function -
 * it expects the next object below it to still be around, and if the
 * object gets merged, it gets removed
 */

void insert_ob_in_map_simple(object *op, mapstruct *m)
{
  object *tmp, *top;

  if (QUERY_FLAG (op, FLAG_FREED)) {
    LOG (llevError, "Trying to insert freed object!\n");
    return;
  }
  if(m==NULL) {
    dump_object(op);
    LOG(llevError,"Trying to insert in null-map!\n%s\n",errmsg);
    return;
  }
  if(out_of_map(m,op->x,op->y)) {
    dump_object(op);
    LOG(llevError,"Trying to insert object outside the map.\n%s\n", errmsg);
    return;
  }
  if(!QUERY_FLAG(op,FLAG_REMOVED)) {
    dump_object(op);
    LOG(llevError,"Trying to insert (map) inserted object.\n%s\n", errmsg);
    return;
  }
  if(op->more!=NULL)
    insert_ob_in_map_simple(op->more,m);
  CLEAR_FLAG(op,FLAG_REMOVED);
  op->ox=op->x,op->oy=op->y;
  op->map=m;
  CLEAR_FLAG(op,FLAG_APPLIED); /* hack for fixing F_APPLIED in items of dead people */
  CLEAR_FLAG(op, FLAG_INV_LOCKED);
  CLEAR_FLAG(op, FLAG_NO_STEAL);
  /* If anything below then */
  if((top=get_map_ob(op->map,op->x,op->y))!=NULL) {
    /*
     * There is at least one object present, so we must figure out the order.
     * First figure out what is on top;
     */

    while (top->above != NULL)
      top = top->above;

    tmp = top;

    /*
     * Rule 0: SEE_ANYWHERE() objects stay on top of the top.
     */
    while(tmp != NULL && QUERY_FLAG(tmp,FLAG_SEE_ANYWHERE) &&
       !QUERY_FLAG(op,FLAG_SEE_ANYWHERE))
          top = tmp, tmp = tmp->below;

    /*
     * Rule 1: Alive objects stay on top.
     */
    while(tmp != NULL && QUERY_FLAG(tmp,FLAG_ALIVE) &&
	!QUERY_FLAG(op,FLAG_ALIVE) && !QUERY_FLAG(op,FLAG_FLYING) &&
          !QUERY_FLAG(op,FLAG_SEE_ANYWHERE))
      top = tmp, tmp = tmp->below;

    /*
     * Link the new object in the two-way list:
     */

    if (top == tmp) {
      /*
       * Simple case, insert new object above tmp.
       */
      if (tmp->above != NULL)
        LOG(llevError, "insert_ob_in_map_simple: top == tmp && tmp->above != NULL.\n");
      else {
        tmp->above = op;
        op->below = tmp;
        op->above = (object *) NULL;
      }
    } else {
      /*
       * Not so simple case, insert between top and tmp.
       */
      if (tmp == NULL) { /* Insert at bottom of map */
        top->below = op;
        op->above = top;
        op->below = (object *) NULL;
        set_map_ob(op->map, op->x, op->y, op);
      } else { /* Insert between top and tmp */
        top->below = op;
        op->above = top;
        tmp->above = op;
        op->below = tmp;
      }
    }
  }
  else
    set_map_ob(op->map,op->x,op->y,op);   /* Tell the map that we're here */

  /* build up linked list of light sources in each map. We do
   * this even for non-dark maps, as down the line we might make
   * a spell/effect which effects the global light of any map.
   * -b.t.
   */
#ifdef USE_LIGHTING
        if(op->glow_radius>0&&light_not_listed(op)) {
		add_light_to_map(op,m);
		update_all_los(m);
	} else if(m->darkness&&(op->glow_radius>0||op->lights)) /* a light moved, check los */
		update_all_los(m);
#endif 
  
  if(op->type==PLAYER)
    op->contr->do_los=1;
  for(tmp=get_map_ob(op->map,op->x,op->y);tmp!=NULL;tmp=tmp->above)
    switch(tmp->type) {
    case PLAYER:
	tmp->contr->socket.update_look=1;
	break;
    }
    update_object(op);
}

/*
 * insert_ob_in_map (op, map, originator):
 * This function inserts the object in the two-way linked list
 * which represents what is on a map.
 * The second argument specifies the map, and the x and y variables
 * in the object about to be inserted specifies the position.
 *
 * originator: Player, monster or other object that caused 'op' to be inserted
 * into 'map'.  May be NULL.
 *
 * Return value:
 *   new object if 'op' was merged with other object
 *   NULL if 'op' was destroyed
 *   just 'op' otherwise
 */

object *insert_ob_in_map (object *op, mapstruct *m, object *originator)
{
  object *tmp, *top;

  if (QUERY_FLAG (op, FLAG_FREED)) {
    LOG (llevError, "Trying to insert freed object!\n");
    return NULL;
  }
  if(m==NULL) {
    dump_object(op);
    LOG(llevError,"Trying to insert in null-map!\n%s\n",errmsg);
    return op;
  }
  if(out_of_map(m,op->x,op->y)) {
    dump_object(op);
    LOG(llevError,"Trying to insert object outside the map.\n%s\n", errmsg);
    return op;
  }
  if(!QUERY_FLAG(op,FLAG_REMOVED)) {
    dump_object(op);
    LOG(llevError,"Trying to insert (map) inserted object.\n%s\n", errmsg);
    return op;
  }
  if(op->more!=NULL) {
    if (insert_ob_in_map(op->more,m,originator) == NULL) {
      if ( ! op->head)
        LOG (llevError, "BUG: insert_ob_in_map(): inserting op->more "
             "killed op\n");
      return NULL;
    }
  }
  CLEAR_FLAG(op,FLAG_REMOVED);
  if(op->nrof)
    for(tmp=get_map_ob(m,op->x,op->y);tmp!=NULL;tmp=tmp->above)
      if (CAN_MERGE(op,tmp))
      {
        op->nrof+=tmp->nrof;
/*        CLEAR_FLAG(op,FLAG_STARTEQUIP);*/
        remove_ob(tmp);
        free_object(tmp);
      }
  op->ox=op->x,op->oy=op->y;
  op->map=m;
  CLEAR_FLAG(op,FLAG_APPLIED); /* hack for fixing F_APPLIED in items of dead people */
  CLEAR_FLAG(op, FLAG_INV_LOCKED);
  if (!QUERY_FLAG(op, FLAG_ALIVE))
    CLEAR_FLAG(op, FLAG_NO_STEAL);
  /* If there are other objects, then */
  if((top=get_map_ob(op->map,op->x,op->y))!=NULL) {
    /*
     * There is at least one object present, so we must figure out the order.
     * First figure out what is on top;
     */

    while (top->above != NULL)
      top = top->above;

    tmp = top;

    /* Re-did this with nested if statements.  In this way, each check does
     * not need to check for the previous state - to me, it makes the code
     * cleaner, and perhaps faster.
     */
    if (!QUERY_FLAG(op, FLAG_SEE_ANYWHERE)) {
      /* See anywhere objects stay on top. */
      while(tmp != NULL && QUERY_FLAG(tmp,FLAG_SEE_ANYWHERE))
          top = tmp, tmp = tmp->below;

      if (!QUERY_FLAG(op, FLAG_ALIVE) && !QUERY_FLAG(op,FLAG_FLYING)) {
	/* Alive objects also stay on to. */
	while(tmp != NULL && QUERY_FLAG(tmp,FLAG_ALIVE))
	  top = tmp, tmp = tmp->below;

        /* Have object 'fall below' other objects that block view, except in
         * cases where the object has a high visibility (high meaning >0)
         */
        if (blocks_view(op->map, op->x, op->y) && (op->face &&
                !op->face->visibility)) {
	  while (tmp != NULL && QUERY_FLAG(tmp, FLAG_BLOCKSVIEW) &&
	    !QUERY_FLAG(op, FLAG_BLOCKSVIEW))
	      top = tmp, tmp = tmp->below;
	}
      }
    }

    /*
     * Link the new object in the two-way list:
     */

    if (top == tmp) {
      /*
       * Simple case, insert new object above tmp.
       */
      if (tmp->above != NULL)
        LOG(llevError, "insert_ob_in_map: top == tmp && tmp->above != NULL.\n");
      else {
        tmp->above = op;
        op->below = tmp;
        op->above = (object *) NULL;
      }
    } else {
      /*
       * Not so simple case, insert between top and tmp.
       */
      if (tmp == NULL) { /* Insert at bottom of map */
        top->below = op;
        op->above = top;
        op->below = (object *) NULL;
        set_map_ob(op->map, op->x, op->y, op);
      } else { /* Insert between top and tmp */
        top->below = op;
        op->above = top;
        tmp->above = op;
        op->below = tmp;
      }
    }
  }
  else
    set_map_ob(op->map,op->x,op->y,op);   /* Tell the map that we're here */

  if(op->type==PLAYER)
    op->contr->do_los=1;
  for(tmp=get_map_ob(op->map,op->x,op->y);tmp!=NULL;tmp=tmp->above)
    switch(tmp->type) {
    case PLAYER:
	tmp->contr->socket.update_look=1;
	break;
    }
  /* build up linked list of light sources in each map. We do
   * this even for non-dark maps, as down the line we might make
   * a spell/effect which effects the global light of any map.
   * -b.t.
   */
#ifdef USE_LIGHTING
        if(op->glow_radius>0&&light_not_listed(op)) {
		add_light_to_map(op,m);
		update_all_los(m);
	} else if(m->darkness&&(op->glow_radius>0||op->lights)) /* a light moved, check los */
		update_all_los(m);
#endif 
    /* Don't know if moving this to the end will break anything.  However,
     * we want to have update_look set above before calling this.
     *
     * check_walk_on() must be after this because code called from
     * check_walk_on() depends on correct map flags (so functions like
     * blocked() and wall() work properly), and these flags are updated by
     * update_object().
     */
    update_object(op);

    /* Only check this if we are the head of the object */
    if ( ! op->head) {
        if (check_walk_on(op, originator))
          return NULL;

        /* If we are a multi part object, lets work our way through the check
         * walk on's.
         */
        for (tmp=op->more; tmp!=NULL; tmp=tmp->more)
            if (check_walk_on (op, originator))
              return NULL;
    }

    return op;
}

/*
 * get_split_ob(ob,nr) splits up ob into two parts.  The part which
 * is returned contains nr objects, and the remaining parts contains
 * the rest (or is removed and freed if that number is 0).
 * On failure, NULL is returned, and the reason put into the
 * global static errmsg array.
 */

object *get_split_ob(object *orig_ob,int nr) {
    object *newob;
    int is_removed = (QUERY_FLAG (orig_ob, FLAG_REMOVED) != 0);

    if(orig_ob->nrof<nr) {
	sprintf(errmsg,"There are only %d %ss.",
            orig_ob->nrof?orig_ob->nrof:1, orig_ob->name);
	return NULL;
    }
    newob=get_object();
    copy_object(orig_ob,newob);
    if((orig_ob->nrof-=nr)<1) {
	if ( ! is_removed)
            remove_ob(orig_ob);
	free_object(orig_ob);
    }
    else if ( ! is_removed) {
	if(orig_ob->env!=NULL)
	    sub_weight (orig_ob->env,orig_ob->weight*nr);
	if (orig_ob->env == NULL && orig_ob->map->in_memory!=MAP_IN_MEMORY) {
	    strcpy(errmsg, "Tried to split object whose map is not in memory.");
	    LOG(llevDebug,
		    "Error, Tried to split object whose map is not in memory.\n");
	    return NULL;
	}
    }
    newob->nrof=nr;
    return newob;
}

/*
 * decrease_ob_nr(object, number) decreases a specified number from
 * the amount of an object.  If the amount reaches 0, the object
 * is subsequently removed and freed.
 *
 * Return value: 'op' if something is left, NULL if the amount reached 0
 */

object *decrease_ob_nr (object *op, int i)
{
    object *tmp;

    if (i == 0)   /* objects with op->nrof require this check */
        return op;

    if (i > op->nrof)
        i = op->nrof;

    if (QUERY_FLAG (op, FLAG_REMOVED))
    {
        op->nrof -= i;
    }
    else if (op->env != NULL)
    {
        tmp = is_player_inv (op->env);
        if (i < op->nrof) {
            sub_weight (op->env, op->weight * i);
            op->nrof -= i;
            if (tmp) {
                (*esrv_send_item_func) (tmp, op);
                (*esrv_update_item_func) (UPD_WEIGHT, tmp, tmp);
            }
        } else {
            remove_ob (op);
            op->nrof = 0;
            if (tmp) {
                (*esrv_del_item_func) (tmp->contr, op->count);
                (*esrv_update_item_func) (UPD_WEIGHT, tmp, tmp);
            }
        }
    }
    else 
    {
	object *above = op->above;

        if (i < op->nrof) {
            op->nrof -= i;
        } else {
            remove_ob (op);
            op->nrof = 0;
        }
	/* Since we just removed op, op->above is null */
        for (tmp = above; tmp != NULL; tmp = tmp->above)
            if (tmp->type == PLAYER) {
                if (op->nrof)
                    (*esrv_send_item_func) (tmp, op);
                else
                    (*esrv_del_item_func) (tmp->contr, op->count);
            }
    }

    if (op->nrof) {
        return op;
    } else {
        free_object (op);
        return NULL;
    }
}

/*
 * add_weight(object, weight) adds the specified weight to an object,
 * and also updates how much the environment(s) is/are carrying.
 */

void add_weight (object *op, signed long weight) {
  while (op!=NULL) {
    if (op->type == CONTAINER) {
      weight=(signed long)(weight*(100-op->stats.Str)/100);
    }
    op->carrying+=weight;
    op=op->env;
  }
}

/*
 * insert_ob_in_ob(op,environment):
 *   This function inserts the object op in the linked list
 *   inside the object environment.
 *
 * Eneq(@csd.uu.se): Altered insert_ob_in_ob to make things picked up enter 
 * the inventory at the last position or next to other objects of the same
 * type.
 * Frank: Now sorted by type, archetype and magic!
 *
 * The function returns now pointer to inserted item, and return value can 
 * be != op, if items are merged. -Tero
 */

object *insert_ob_in_ob(object *op,object *where) {
  object *tmp, *otmp;

  if(!QUERY_FLAG(op,FLAG_REMOVED)) {
    dump_object(op);
    LOG(llevError,"Trying to insert (ob) inserted object.\n%s\n", errmsg);
    return op;
  }
  if(where==NULL) {
    dump_object(op);
    LOG(llevError,"Trying to put object in NULL.\n%s\n", errmsg);
    return op;
  }
  if (where->head) {
    LOG(llevDebug, 
	"Warning: Tried to insert object wrong part of multipart object.\n");
    where = where->head;
  }
  if (op->more) {
    LOG(llevError, "Tried to insert multipart object %s (%d)\n",
        op->name, op->count);
    return op;
  }
  CLEAR_FLAG(op, FLAG_REMOVED);
  if(op->nrof) {
    for(tmp=where->inv;tmp!=NULL;tmp=tmp->below)
      if ( CAN_MERGE(tmp,op) ) {
	/* return the original object and remove inserted object
           (client needs the original object) */
        tmp->nrof += op->nrof;
	/* Weight handling gets pretty funky.  Since we are adding to
	 * tmp->nrof, we need to increase the weight.
	 */
	add_weight (where, op->weight*op->nrof);
        SET_FLAG(op, FLAG_REMOVED);
        free_object(op); /* free the inserted object */
        op = tmp;
        remove_ob (op); /* and fix old object's links */
        CLEAR_FLAG(op, FLAG_REMOVED);
	break;
      }

    /* I assume combined objects have no inventory
     * We add the weight - this object could have just been removed
     * (if it was possible to merge).  calling remove_ob will subtract
     * the weight, so we need to add it in again, since we actually do
     * the linking below
     */
    add_weight (where, op->weight*op->nrof);
  } else
    add_weight (where, (op->weight+op->carrying));

  otmp=is_player_inv(where);
  if (otmp&&otmp->contr!=NULL) {
    if (!QUERY_FLAG(otmp,FLAG_NO_FIX_PLAYER))
      fix_player(otmp);
  }

  op->map=NULL;
  op->env=where;
  op->above=NULL;
  op->below=NULL;
  op->x=0,op->y=0;
  op->ox=0,op->oy=0;

#ifdef USE_LIGHTING /* reset the light list and los of the players on the map */
  if(op->glow_radius>0&&where->map)
  {
#ifdef DEBUG_LIGHTS
      LOG(llevDebug, " insert_ob_in_ob(): got %s to insert in map/op\n",
	op->name);
#endif /* DEBUG_LIGHTS */ 
      add_light_to_list(op,where);
      if(light_not_listed(op)) add_light_to_map(op,where->map);
      if(where->map->darkness&&where->map->players) update_all_los(where->map);
  }
#endif /* USE_LIGHTING */ 

  /* Client has no idea of ordering so lets not bother ordering it here.
   * It sure simplifies this function...
   */
  if (where->inv==NULL)
      where->inv=op;
  else {
      op->below = where->inv;
      op->below->above = op;
      where->inv = op;
  }
  return op;
}

/*
 * Checks if any objects which has the WALK_ON() (or FLY_ON() if the
 * object is flying) flag set, will be auto-applied by the insertion
 * of the object into the map (applying is instantly done).
 * Any speed-modification due to SLOW_MOVE() of other present objects
 * will affect the speed_left of the object.
 *
 * originator: Player, monster or other object that caused 'op' to be inserted
 * into 'map'.  May be NULL.
 *
 * Return value: 1 if 'op' was destroyed, 0 otherwise.
 */
 /* 4-21-95 added code to check if appropriate skill was readied - this will
  * permit faster movement by the player through this terrain. -b.t.
  */

int check_walk_on (object *op, object *originator)
{
    object *tmp;
    tag_t tag;

    if(QUERY_FLAG(op,FLAG_NO_APPLY))
	return 0;
    tag = op->count;
    for(tmp=op->below;tmp!=NULL;tmp=tmp->below) {
	if (tmp==tmp->below)	/* if we have a map loop, exit */
	    break;
	if(QUERY_FLAG(tmp,FLAG_SLOW_MOVE)&&!QUERY_FLAG(op,FLAG_FLYING)) {
	    float diff;

	    diff=(SLOW_PENALTY(tmp)*FABS(op->speed));
	    if (op->type==PLAYER) {
		if ((QUERY_FLAG(tmp,FLAG_IS_HILLY) && find_skill(op,SK_CLIMBING)) ||
		    (QUERY_FLAG(tmp,FLAG_IS_WOODED) && find_skill(op,SK_WOODSMAN)))  {
			diff=diff/4.0;
		}
	    }
	    op->speed_left -= diff;
	}
	if(QUERY_FLAG(op,FLAG_FLYING)?QUERY_FLAG(tmp,FLAG_FLY_ON):
	   QUERY_FLAG(tmp,FLAG_WALK_ON)) {
	    move_apply_func (tmp, op, originator);
            if (was_destroyed (op, tag))
              return 1;
	}
    }
    return 0;
}

/*
 * present_arch(arch, map, x, y) searches for any objects with
 * a matching archetype at the given map and coordinates.
 * The first matching object is returned, or NULL if none.
 */

object *present_arch(archetype *at, mapstruct *m, int x, int y) {
  object *tmp;
  if(m==NULL || out_of_map(m,x,y)) {
    LOG(llevError,"Present_arch called outside map.\n");
    return NULL;
  }
  for(tmp=get_map_ob(m,x,y); tmp != NULL; tmp = tmp->above)
    if(tmp->arch == at)
      return tmp;
  return NULL;
}

/*
 * present(type, map, x, y) searches for any objects with
 * a matching type variable at the given map and coordinates.
 * The first matching object is returned, or NULL if none.
 */

object *present(unsigned char type,mapstruct *m, int x,int y) {
  object *tmp;
  if(out_of_map(m,x,y)) {
    LOG(llevError,"Present called outside map.\n");
    return NULL;
  }
  for(tmp=get_map_ob(m,x,y);tmp!=NULL;tmp=tmp->above)
    if(tmp->type==type)
      return tmp;
  return NULL;
}

/*
 * present_in_ob(type, object) searches for any objects with
 * a matching type variable in the inventory of the given object.
 * The first matching object is returned, or NULL if none.
 */

object *present_in_ob(unsigned char type,object *op) {
  object *tmp;
  for(tmp=op->inv;tmp!=NULL;tmp=tmp->below)
    if(tmp->type==type)
      return tmp;
  return NULL;
}

/*
 * present_arch_in_ob(archetype, object) searches for any objects with
 * a matching archetype in the inventory of the given object.
 * The first matching object is returned, or NULL if none.
 */

object *present_arch_in_ob(archetype *at, object *op)  {
  object *tmp;
  for(tmp=op->inv;tmp!=NULL;tmp=tmp->below)
    if( tmp->arch == at)
      return tmp;
  return NULL;
}

/*
 * set_cheat(object) sets the cheat flag (WAS_WIZ) in the object and in
 * all it's inventory (recursively).
 * If checksums are used, a player will get set_cheat called for
 * him/her-self and all object carried by a call to this function.
 */

void set_cheat(object *op) {
  object *tmp;
  SET_FLAG(op, FLAG_WAS_WIZ);
  if(op->inv)
    for(tmp = op->inv; tmp != NULL; tmp = tmp->below)
      set_cheat(tmp);
}

/*
 * find_free_spot(archetype, map, x, y, start, stop) will search for
 * a spot at the given map and coordinates which will be able to contain
 * the given archetype.  start and stop specifies how many squares
 * to search (see the freearr_x/y[] definition).
 * It returns a random choice among the alternatives found.
 * start and stop are where to start relative to the free_arr array (1,9
 * does all 4 immediate directions).  This returns the index into the
 * array of the free spot, -1 if no spot available (dir 0 = x,y)
 * Note - this only checks to see if there is space for the head of the
 * object - if it is a multispace object, this should be called for all
 * pieces.
 */

int find_free_spot(archetype *at, mapstruct *m,int x,int y,int start,int stop) {
  int i,index=0;
  static int altern[SIZEOFFREE];
  for(i=start;i<stop;i++) {
    /* Surprised the out_of_map check was missing. Without it, we may
     * end up accessing garbage, which may say a space is free
     */
    if (arch_out_of_map(at, m, x+freearr_x[i],y+freearr_y[i])) continue;
    if(!arch_blocked(at,m,x+freearr_x[i],y+freearr_y[i]))
      altern[index++]=i;
    else if(wall(m,x+freearr_x[i],y+freearr_y[i])&&maxfree[i]<stop)
      stop=maxfree[i];
  }
  if(!index) return -1;
  return altern[RANDOM()%index];
}

/*
 * find_first_free_spot(archetype, mapstruct, x, y) works like
 * find_free_spot(), but it will search max number of squares.
 * But it will return the first available spot, not a random choice.
 * Changed 0.93.2: Have it return -1 if there is no free spot available.
 */

int find_first_free_spot(archetype *at, mapstruct *m,int x,int y) {
  int i;
  for(i=0;i<SIZEOFFREE;i++) {
    if (out_of_map(m,x+freearr_x[i],y+freearr_y[i])) continue;
    if(!arch_blocked(at,m,x+freearr_x[i],y+freearr_y[i]))
      return i;
  }
  return -1;
}

/*
 * find_dir(map, x, y, exclude) will search some close squares in the
 * given map at the given coordinates for live objects.
 * It will not considered the object given as exlude among possible
 * live objects.
 * It returns the direction toward the first/closest live object if finds
 * any, otherwise 0.
 */

int find_dir(mapstruct *m, int x, int y, object *exclude) {
  int i,max=SIZEOFFREE;
  object *tmp;
  if (exclude && exclude->head)
    exclude = exclude->head;

  for(i=1;i<max;i++) {
    if(wall(m, x+freearr_x[i],y+freearr_y[i]))
      max=maxfree[i];
    else {
      tmp=get_map_ob(m,x+freearr_x[i],y+freearr_y[i]);
      while(tmp!=NULL && ((tmp!=NULL&&!QUERY_FLAG(tmp,FLAG_MONSTER)&&
	tmp->type!=PLAYER) || (tmp == exclude || 
	(tmp->head && tmp->head == exclude))))
	        tmp=tmp->above;
      if(tmp!=NULL)
        return freedir[i];
    }
  }
  return 0;
}

/*
 * distance(object 1, object 2) will return the square of the
 * distance between the two given objects.
 */

int distance(object *ob1,object *ob2) {
  int i;
  i= (ob1->x - ob2->x)*(ob1->x - ob2->x)+
         (ob1->y - ob2->y)*(ob1->y - ob2->y);
  return i;
}

/*
 * find_dir_2(delta-x,delta-y) will return a direction in which
 * an object which has subtracted the x and y coordinates of another
 * object, needs to travel toward it.
 */

int find_dir_2(int x, int y) {
  int q;
  if(!y)
    q= -300*x;
  else
    q=x*100/y;
  if(y>0) {
    if(q < -242)
      return 3 ;
    if (q < -41)
      return 2 ;
    if (q < 41)
      return 1 ;
    if (q < 242)
      return 8 ;
    return 7 ;
  }
  if (q < -242)
    return 7 ;
  if (q < -41)
    return 6 ;
  if (q < 41)
    return 5 ;
  if (q < 242)
    return 4 ;
  return 3 ;
}

/*
 * absdir(int): Returns a number between 1 and 8, which represent
 * the "absolute" direction of a number (it actually takes care of
 * "overflow" in previous calculations of a direction).
 */

int absdir(int d) {
  while(d<1) d+=8;
  while(d>8) d-=8;
  return d;
}

/*
 * dirdiff(dir1, dir2) returns how many 45-degrees differences there is
 * between two directions (which are expected to be absolute (see absdir())
 */

int dirdiff(int dir1, int dir2) {
  int d;
  d = abs(dir1 - dir2);
  if(d>4)
    d = 8 - d;
  return d;
}

/*
 * can_pick(picker, item): finds out if an object is possible to be
 * picked up by the picker.  Returnes 1 if it can be
 * picked up, otherwise 0.
 *
 * Cf 0.91.3 - don't let WIZ's pick up anything - will likely cause
 * core dumps if they do.
 *
 * Add a check so we can't pick up invisible objects (0.93.8)
 */

int can_pick(object *who,object *item) {
  return /*QUERY_FLAG(who,FLAG_WIZ)||*/
         (item->weight>0&&!QUERY_FLAG(item,FLAG_NO_PICK)&&
	 !QUERY_FLAG(item,FLAG_ALIVE)&&!item->invisible && 
          (who->type==PLAYER||item->weight<who->weight/3));
}


/*
 * create clone from object to another
 */
object *ObjectCreateClone (object *asrc) {
    object *dst = NULL,*tmp,*src,*part,*prev, *item;

    if(!asrc) return NULL;
    src = asrc;
    if(src->head)
        src = src->head;

    prev = NULL;
    for(part = src; part; part = part->more) {
        tmp = get_object();
        copy_object(part,tmp);
        tmp->x -= src->x;
        tmp->y -= src->y;
        if(!part->head) {
            dst = tmp;
            tmp->head = NULL;
        } else {
            tmp->head = dst;
        }
        tmp->more = NULL;
        if(prev) 
            prev->more = tmp;
        prev = tmp;
    }
    /*** copy inventory ***/
    for(item = src->inv; item; item = item->below) {
	(void) insert_ob_in_ob(ObjectCreateClone(item),dst);
    }

    return dst;
}

int was_destroyed (object *op, tag_t old_tag)
{
    /* checking for FLAG_FREED isn't necessary, but makes this function more
     * robust */
    return op->count != old_tag || QUERY_FLAG (op, FLAG_FREED);
}

/*** end of object.c ***/
