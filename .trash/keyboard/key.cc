#include "key.h"

const char *keyToStr(const Key key) {
   switch(key) {
   case Left:
	   return "Left";
   case Up:
	   return "Up";
   case Right:
	   return "Right";
   case Down:
	   return "Down";
   default:
	   return "";
   }
}
