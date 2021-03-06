#ifndef PARSE_H__
#define PARSE_H__

// Register response states
enum register_response_state {UserExists = 0, RegisterSuccess = 1, NULLRegisterFailure = 2};

// Login response states
enum login_response_state {UserDNE = 0, IncorrectPassword = 1, LoginSuccess = 2, NULLLoginFailure = 3};// Register response states

// Add user history response states
enum add_history_response_state {NoSuchUser = 0, AddOK = 1, NULLAddFailure = 3};

// Remove user history response states
enum remove_history_response_state {NoSuchTranslation = 0, RemoveOK = 1, NULLRemoveFailure = 2};

// Open socket response states
enum open_sock_response_state {OpenSockOK = 0, OpenSockERR = 1, NULLOpenSockFailure = 2};

// Location retrieval response states
enum get_location_response_state {GetLocationOther  = 0, GetLocationFrance  = 1, GetLocationSpain = 2,
                                  GetLocationGermany  = 3, GetLocationItaly = 4, NULLLocationFailure = 5};



// ------------------------------------- UTILITY functions ---------------------------------

/**
 * Check to see if the list associated with "objects" key is empty.
 * @param str Pointer to the string to read the JSON response from
 *            This pointer must be set to  immediately after the "objects" key location
 */
int check_empty_array(char *str);

/**
 * Get the key or value in a JSON object limited by double-quotation marks
 * @param str Pointer to the string to read the JSON response from
 * @param buf Pointer to the buffer in which key or value is stored
 */
char * get_Q_to_Q(char *str, char *buf);

/**
 * Get objects from a JSON response limited by curly brace marks
 * @param str Pointer to the string to read the JSON response from
 * @param buf Pointer to the buffer in which key or value is stored
 */
char * get_CB_to_CB(char *str, char * buf, int * bytes_written);

/**
 * Replaced all the spaces in the string with %20, for use in HTTP requests parameters
 * @param original Pointer to a string that may or may not contain spaces
 * @return Pointer to a malloc'd string. Remember to free after usage.
 */
char * spaces_replaced(char * original);

// --------------------------------------------------------------------------------------------



// --------------------------- Translation JSON Parsing and Helpers ---------------------------------

/**
 * Struct associated with the outermost level of keys in the server JSON response
 */
typedef struct outObjs
{
    char name[1024];
    char val[1024];
    int objectCount;
    struct objTransNode * head;
} outObjs;

/**
 * Struct associated with the nodes of the linkedlist, each containing
 * a native language word, a translated word and a reference to the next node
 */
typedef struct objTransNode
{
    struct objTransNode * next;
    char native[100];
    char translated[100];
} objTransNode;


/**
 * Struct to containt the parse JSON object response for each translation
 * request with native and target languages as well as a reference to 
 * the obtained objects array (stored as a linkedlist)
 */
typedef struct topLevelParsed
{
	outObjs * objects;
	outObjs * targetLanguage;
	outObjs * nativeLanguage;
} topLevelParsed;

/**
 * Parses the buffer received from the translate call, and returns a malloc'd topLevelParsed struct
 * that contains all information received from the server
 * 
 * @param str Pointer to a string buffer that containst the server response
 */
topLevelParsed * parse_translation_buffer(char * str);

/**
 * Gets the values for the key found in the JSON object and appropriately parses the associated
 * value. This must be called for each key obtained in the outermost level in the JSON response.
 *
 * For native and translated language information, simply adds the string to the structs
 * For objects, creates a linkedlist of all objects found, with native and translated information
 *
 * @param str Pointer to a pointer to the string to read the JSON response from
 *            This pointer must be set to  immediately after the relevant key location
 * @param nativeLanguage Pointer to the outObjs struct associated with the native language information
 * @param targetLanguage Pointer to the outObjs struct associated with the translated language information
 * @param nativeLanguage Pointer to the outObjs struct associated with the objects found information
 * @param param Pointer to the string associated with the key value read, for parsing purposes
 */
int get_param(char ** str, outObjs *nativeLanguage, outObjs *targetLanguage, outObjs *objects, char *param);

/**
 * Populate the struct associated with nodes in the linked list of objects
 * @param str Pointer to a pointer to the string to read the JSON response from
 * @param buf Pointer to the buffer containing the key value
 * @param node Pointer to the object node whose fields need to be populated
 */
int get_obj_params(char ** str, char * buf, objTransNode * node);

/**
 * Properly frees all the memory in the topLevelParsed struct, given a pointer to the struct
 * 
 * @param toFree The topLevelParsed struct of translations to free
 */
void free_struct(topLevelParsed * toFree);

/**
 * Frees all the linked list nodes associated with the "objects" list in the JSON reponse
 * 
 * @param objects The objects outObjs pointer to allow us to access the linkedlist of objects
 */
void free_nodes(outObjs * objects);

// ------------------------------------------------------------------------------------------------------------

// ------------------------- Screen Interaction Server Response Parsing ----------------------

/**
 * Parses the buffer received from the user registration call, and returns the status
 * enumeration based on response from the server
 * 
 * @param str Pointer to a string buffer that containst the server response
 * @return enum to express the status indicated by server response
 */
int parse_register_buffer(char * str);

/**
 * Parses the buffer received from the user login call, and returns the status
 * enumeration based on response from the server
 * 
 * @param str Pointer to a string buffer that containst the server response
 * @return enum to express the status indicated by server response
 */
int parse_login_buffer(char * str);

/**
 * Parses the buffer received from the history addition call, and returns the status
 * enumeration based on response from the server
 * 
 * @param str Pointer to a string buffer that containst the server response
 * @return enum to express the status indicated by server response
 */
int parse_add_history_buffer(char * str);

/**
 * Parses the buffer received from the history removal call, and returns the status
 * enumeration based on response from the server
 * 
 * @param str Pointer to a string buffer that containst the server response
 * @return enum to express the status indicated by server response
 */
int parse_remove_history_buffer(char * str);

/**
 * Parses the buffer received from the open socket call, and returns the status
 * enumeration based on response from the server
 * 
 * @param str Pointer to a string buffer that containst the server response
 * @return enum to express the status indicated by server response
 */
int parse_open_sock_buffer(char * str);

/**
 * Parses the buffer received from the get location call, and returns the status
 * enumeration based on response from the server
 * 
 * @param str Pointer to a string buffer that containst the server response
 * @return enum to express the status indicated by server response
 */
int parse_location_buffer(char * str);

// ---------------------------------------------------------------------------------------

// --------------------------- History Parsing and Helper functions ----------------------

/**
 * Struct associated with history object in server response for user translation history
 * Contains count of translations and head of linkedlist of translations
 */
typedef struct historyObj
{
    int translationCount;
    struct historyObjectNode * head;
} historyObj;

/**
 * Struct associated with individual user history translations (linked list node)
 */
typedef struct historyObjectNode
{
    struct historyObjectNode * next;
    char native_language[10];
    char target_language[10];
    char native_word[100];
    char target_word[100];
} historyObjectNode;


/**
 * Parses the buffer received from the get user history call, and returns the status
 * enumeration based on response from the server
 * 
 * @param str Pointer to a string buffer that containst the server response
 */
historyObj * parse_history_buffer(char * str);

/**
 * Populate the struct associated with nodes in the linked list of objects
 * 
 * @param str Pointer to a pointer to the string to read the JSON response from
 * @param buf Pointer to the buffer containing the key value
 * @param node Pointer to the object node whose fields need to be populated
 * @return an integer to indicate the type of key of value pair read
 */
int get_history_params(char ** str, char * buf, historyObjectNode * node);


/**
 * Deletes a node from the linked list in a historyObj struct, at position index
 * Helper for removal from history
 * 
 * @param history historyObj struct pointing to the appropriate linkedlist
 * @param index integer to indicated the index at which to remove the node from linked list
 */
void remove_history_node(historyObj * history, int index);


/**
 * Properly frees all the memory in the historyObj struct, given a pointer to the struct
 * 
 * @param history historyObj struct to free
 */
void free_translation_history(historyObj * history);

/**
 * Properly frees all the memory for the nodes of the linked list in the historyObj struct, 
 * given a pointer to the struct
 * 
 * @param history historyObj struct containing reference to appropriate likedist
 */
void free_history_nodes(historyObj * history);

// ----------------------------------------------------------------------------------------

#endif /* PARSE_H__*/
