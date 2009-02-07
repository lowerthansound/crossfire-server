/*
 * static char *rcsid_ob_methods =
 *   "$Id: build_map.c 5057 2006-10-29 07:50:09Z mwedel $";
 */
/*
    CrossFire, A Multiplayer game for X-windows

    Copyright (C) 2006 Mark Wedel & Crossfire Development Team
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

    The authors can be reached via e-mail to crossfire-devel@real-time.com
*/

/**
 * @file
 * Those functions deal with the object/type system.
 */

#include <global.h>
#include <ob_methods.h>
#include <ob_types.h>

#ifndef __CEXTRACT__
#include <sproto.h>
#endif

/*
 * The following functions are meant for calling methods. No actual behavoir
 * logic should be contained in this code. Code in the common/ directory should
 * be used for logic common to all types, and should always be called by
 * individual method code (i.e. all apply methods should call 'can_apply' from
 * common/). Defaults for all types should not be put here either, as that code
 * belongs in the common/ directory also, referenced to by base_type.
 */

/**
 * Applies an object.
 * @param op The object to apply
 * @param applier The object that executes the apply action
 * @param aflags Special (always apply/unapply) flags
 * @retval 0 A player or monster can't apply objects of that type
 * @retval 1 has been applied, or there was an error applying the object
 * @retval 2 objects of that type can't be applied if not in inventory
 */
method_ret ob_apply(object *op, object *applier, int aflags) {
    method_ret ret;
    ob_methods *methods;

    for (methods = &type_methods[op->type]; methods; methods = methods->fallback) {
        if (methods->apply) {
            ret = methods->apply(methods, op, applier, aflags);
            if (ret != METHOD_UNHANDLED)
                return ret;
        }
    }
    return METHOD_UNHANDLED;
}

/**
 * Processes an object, giving it the opportunity to move or react.
 * Note: The return value of ob_process doesn't seem to be used anymore.
 * @param op The object to process
 * @retval METHOD_UNHANDLED if the process method does not exist for that objec,
 */
method_ret ob_process(object *op) {
    method_ret ret;
    ob_methods *methods;

    for (methods = &type_methods[op->type]; methods; methods = methods->fallback) {
        if (methods->process) {
            ret = methods->process(methods, op);
            if (ret != METHOD_UNHANDLED)
                return ret;
        }
    }
    return METHOD_UNHANDLED;
}

/**
 * Returns the description of an object, as seen by the given observer.
 * @param op The object to describe
 * @param observer The object to which the description is made
 * @param buf Buffer that will contain the description
 * @param size Size of buf
 * @return
 * buf.
 */
char *ob_describe(const object *op, const object *observer, char *buf, size_t size) {
    ob_methods *methods;

    for (methods = &type_methods[op->type]; methods; methods = methods->fallback) {
        if (methods->describe) {
            methods->describe(methods, op, observer, buf, size);
            return buf;
        }
    }
    buf[0] = '\0';
    return buf;
}

/**
 * Makes an object move on top of another one.
 * @param op The object over which to move
 * @param victim The object moving over op
 * @param originator The object that is the cause of the move
 * @retval METHOD_UNHANDLED if the process method does not exist for that object
 */
method_ret ob_move_on(object *op, object *victim, object *originator) {
    method_ret ret;
    ob_methods *methods;

    for (methods = &type_methods[op->type]; methods; methods = methods->fallback) {
        if (methods->move_on) {
            ret = methods->move_on(methods, op, victim, originator);
            if (ret != METHOD_UNHANDLED)
                return ret;
        }
    }
    return METHOD_UNHANDLED;
}

/**
 * An object is triggered by another one.
 * @param op The object being triggered
 * @param cause The object that is the cause of the trigger
 * @param state trigger state, 0 for released, other for pushed
 * @retval METHOD_UNHANDLED if the process method does not exist for that object
 * @todo check the exact state values/meaning
 */
method_ret ob_trigger(object *op, object *cause, int state) {
    method_ret ret;
    ob_methods *methods;

    for (methods = &type_methods[op->type]; methods; methods = methods->fallback) {
        if (methods->trigger) {
            ret = methods->trigger(methods, op, cause, state);
            if (ret != METHOD_UNHANDLED)
                return ret;
        }
    }
    return METHOD_UNHANDLED;
}
