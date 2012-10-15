#include "lua_utils.h"

namespace {
	void handleUserdata(lua_State* L, int index) {

		void* p = lua_touserdata(L, index);

		if (p != nullptr) {
			if (lua_getmetatable(L, index)) {
				lua_pushvalue(L, LUA_REGISTRYINDEX);

				/* table is in the stack at index 't' */
				bool found = false;
				lua_pushnil(L);
				while (lua_next(L, -2) != 0) {
					if (lua_istable(L, -1)) {
						if (lua_rawequal(L, -1, -4)) {
							printf("%d: lua_user_data, metatable: %s\n", index, lua_tostring(L, -2));
							lua_pop(L, 4); // pop the key-value pair, the registry and the metatable
							return;
						}
					}
					lua_pop(L, 1); // pop the value
				}
				lua_pop(L, 2); // pop the registry and the metatable
			}
		}

		printf("%d: lua_user_data\n", index);
	}
}

namespace LuaUtils {




	void stackDump(lua_State *L)
	{
		int top = lua_gettop(L);
		for (int i = 1; i <= top; i++) {
			int t = lua_type(L, i);
			switch (t) {
				case LUA_TSTRING:
					printf("%d: lua_string: %s\n", i, lua_tostring(L, i));
					break;
				case LUA_TBOOLEAN:
					printf("%d: lua_boolean: %d\n", i, lua_toboolean(L, i));
					break;
				case LUA_TNUMBER:
					printf("%d: lua_number: %f\n", i, lua_tonumber(L, i));
					break;
				case LUA_TNIL:
					printf("%d: lua_nil\n", i);
					break;
				case LUA_TTABLE:
					printf("%d: lua_table\n", i);
					break;
				case LUA_TFUNCTION:
					printf("%d: lua_function\n", i);
					break;
				case LUA_TUSERDATA:
					handleUserdata(L, i);
					break;
				case LUA_TTHREAD:
					printf("%d: lua_thread\n", i);
					break;
				default:
					printf("%d: unkonwn type\n", i);
					break;
			}
		}
	}

}
