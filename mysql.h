#ifndef _MYSQL_H_
#define _MYSQL_H_

#include <cstddef>

#define MYSQL_ERRMSG_SIZE   512
#define SQLSTATE_LENGTH 5

struct st_ma_pvio;
typedef struct st_ma_pvio MARIADB_PVIO;

#if defined(_WIN64)
#define my_socket unsigned long long
#elif defined(_WIN32)
#define my_socket unsigned int
#else
typedef int my_socket;
#endif

typedef struct st_list
{
    struct st_list *prev,*next;
    void *data;
} LIST;

#define CLIENT_MYSQL          1
#define CLIENT_FOUND_ROWS	    2	/* Found instead of affected rows */
#define CLIENT_LONG_FLAG	    4	/* Get all column flags */
#define CLIENT_CONNECT_WITH_DB	    8	/* One can specify db on connect */
#define CLIENT_NO_SCHEMA	   16	/* Don't allow database.table.column */
#define CLIENT_COMPRESS		   32	/* Can use compression protocol */
#define CLIENT_ODBC		   64	/* Odbc client */
#define CLIENT_LOCAL_FILES	  128	/* Can use LOAD DATA LOCAL */
#define CLIENT_IGNORE_SPACE	  256	/* Ignore spaces before '(' */
#define CLIENT_INTERACTIVE	  1024	/* This is an interactive client */
#define CLIENT_SSL                2048     /* Switch to SSL after handshake */
#define CLIENT_IGNORE_SIGPIPE     4096     /* IGNORE sigpipes */
#define CLIENT_TRANSACTIONS	  8192	/* Client knows about transactions */
/* added in 4.x */
#define CLIENT_PROTOCOL_41         512
#define CLIENT_RESERVED          16384
#define CLIENT_SECURE_CONNECTION 32768  
#define CLIENT_MULTI_STATEMENTS  (1UL << 16)
#define CLIENT_MULTI_RESULTS     (1UL << 17)
#define CLIENT_PS_MULTI_RESULTS  (1UL << 18)
#define CLIENT_PLUGIN_AUTH       (1UL << 19)
#define CLIENT_CONNECT_ATTRS     (1UL << 20)
#define CLIENT_CAN_HANDLE_EXPIRED_PASSWORDS (1UL << 22)
#define CLIENT_SESSION_TRACKING  (1UL << 23)
#define CLIENT_PROGRESS          (1UL << 29) /* client supports progress indicator */
#define CLIENT_PROGRESS_OBSOLETE  CLIENT_PROGRESS 
#define CLIENT_SSL_VERIFY_SERVER_CERT (1UL << 30)
#define CLIENT_REMEMBER_OPTIONS  (1UL << 31)

enum mysql_option 
{
    MYSQL_OPT_CONNECT_TIMEOUT, 
    MYSQL_OPT_COMPRESS,
    MYSQL_OPT_NAMED_PIPE,
    MYSQL_INIT_COMMAND,
    MYSQL_READ_DEFAULT_FILE,
    MYSQL_READ_DEFAULT_GROUP,
    MYSQL_SET_CHARSET_DIR,
    MYSQL_SET_CHARSET_NAME,
    MYSQL_OPT_LOCAL_INFILE,
    MYSQL_OPT_PROTOCOL,
    MYSQL_SHARED_MEMORY_BASE_NAME,
    MYSQL_OPT_READ_TIMEOUT,
    MYSQL_OPT_WRITE_TIMEOUT,
    MYSQL_OPT_USE_RESULT,
    MYSQL_OPT_USE_REMOTE_CONNECTION,
    MYSQL_OPT_USE_EMBEDDED_CONNECTION,
    MYSQL_OPT_GUESS_CONNECTION,
    MYSQL_SET_CLIENT_IP,
    MYSQL_SECURE_AUTH,
    MYSQL_REPORT_DATA_TRUNCATION,
    MYSQL_OPT_RECONNECT,
    MYSQL_OPT_SSL_VERIFY_SERVER_CERT,
    MYSQL_PLUGIN_DIR,
    MYSQL_DEFAULT_AUTH,
    MYSQL_OPT_BIND,
    MYSQL_OPT_SSL_KEY,
    MYSQL_OPT_SSL_CERT,
    MYSQL_OPT_SSL_CA,
    MYSQL_OPT_SSL_CAPATH,
    MYSQL_OPT_SSL_CIPHER,
    MYSQL_OPT_SSL_CRL,
    MYSQL_OPT_SSL_CRLPATH,
    /* Connection attribute options */
    MYSQL_OPT_CONNECT_ATTR_RESET,
    MYSQL_OPT_CONNECT_ATTR_ADD,
    MYSQL_OPT_CONNECT_ATTR_DELETE,
    MYSQL_SERVER_PUBLIC_KEY,
    MYSQL_ENABLE_CLEARTEXT_PLUGIN,
    MYSQL_OPT_CAN_HANDLE_EXPIRED_PASSWORDS,
    MYSQL_OPT_SSL_ENFORCE,
    MYSQL_OPT_MAX_ALLOWED_PACKET,
    MYSQL_OPT_NET_BUFFER_LENGTH,
    MYSQL_OPT_TLS_VERSION,

    /* MariaDB specific */
    MYSQL_PROGRESS_CALLBACK=5999,
    MYSQL_OPT_NONBLOCK,
    /* MariaDB Connector/C specific */
    MYSQL_DATABASE_DRIVER=7000,
    MARIADB_OPT_SSL_FP,             /* deprecated, use MARIADB_OPT_TLS_PEER_FP instead */
    MARIADB_OPT_SSL_FP_LIST,        /* deprecated, use MARIADB_OPT_TLS_PEER_FP_LIST instead */
    MARIADB_OPT_TLS_PASSPHRASE,     /* passphrase for encrypted certificates */
    MARIADB_OPT_TLS_CIPHER_STRENGTH,
    MARIADB_OPT_TLS_VERSION,
    MARIADB_OPT_TLS_PEER_FP,            /* single finger print for server certificate verification */
    MARIADB_OPT_TLS_PEER_FP_LIST,       /* finger print white list for server certificate verification */
    MARIADB_OPT_CONNECTION_READ_ONLY,
    MYSQL_OPT_CONNECT_ATTRS,        /* for mysql_get_optionv */
    MARIADB_OPT_USERDATA,
    MARIADB_OPT_CONNECTION_HANDLER,
    MARIADB_OPT_PORT,
    MARIADB_OPT_UNIXSOCKET,
    MARIADB_OPT_PASSWORD,
    MARIADB_OPT_HOST,
    MARIADB_OPT_USER,
    MARIADB_OPT_SCHEMA,
    MARIADB_OPT_DEBUG,
    MARIADB_OPT_FOUND_ROWS,
    MARIADB_OPT_MULTI_RESULTS,
    MARIADB_OPT_MULTI_STATEMENTS,
    MARIADB_OPT_INTERACTIVE,
    MARIADB_OPT_PROXY_HEADER,
    MARIADB_OPT_IO_WAIT
};

enum enum_field_types
{
    MYSQL_TYPE_DECIMAL, MYSQL_TYPE_TINY,
    MYSQL_TYPE_SHORT,  MYSQL_TYPE_LONG,
    MYSQL_TYPE_FLOAT,  MYSQL_TYPE_DOUBLE,
    MYSQL_TYPE_NULL,   MYSQL_TYPE_TIMESTAMP,
    MYSQL_TYPE_LONGLONG,MYSQL_TYPE_INT24,
    MYSQL_TYPE_DATE,   MYSQL_TYPE_TIME,
    MYSQL_TYPE_DATETIME, MYSQL_TYPE_YEAR,
    MYSQL_TYPE_NEWDATE, MYSQL_TYPE_VARCHAR,
    MYSQL_TYPE_BIT,
    /*
      the following types are not used by client,
      only for mysqlbinlog!!
    */
    MYSQL_TYPE_TIMESTAMP2,
    MYSQL_TYPE_DATETIME2,
    MYSQL_TYPE_TIME2,
    /* --------------------------------------------- */
    MYSQL_TYPE_JSON=245,
    MYSQL_TYPE_NEWDECIMAL=246,
    MYSQL_TYPE_ENUM=247,
    MYSQL_TYPE_SET=248,
    MYSQL_TYPE_TINY_BLOB=249,
    MYSQL_TYPE_MEDIUM_BLOB=250,
    MYSQL_TYPE_LONG_BLOB=251,
    MYSQL_TYPE_BLOB=252,
    MYSQL_TYPE_VAR_STRING=253,
    MYSQL_TYPE_STRING=254,
    MYSQL_TYPE_GEOMETRY=255,
    MAX_NO_FIELD_TYPES
};
enum mysql_status
{
    MYSQL_STATUS_READY,
    MYSQL_STATUS_GET_RESULT,
    MYSQL_STATUS_USE_RESULT,
    MYSQL_STATUS_QUERY_SENT,
    MYSQL_STATUS_SENDING_LOAD_DATA,
    MYSQL_STATUS_FETCHING_DATA,
    MYSQL_STATUS_NEXT_RESULT_PENDING,
    MYSQL_STATUS_QUIT_SENT, /* object is "destroyed" at this stage */
    MYSQL_STATUS_STMT_RESULT
};

struct st_mysql_options
{
    unsigned int connect_timeout, read_timeout, write_timeout;
    unsigned int port, protocol;
    unsigned long client_flag;
    char *host,*user,*password,*unix_socket,*db;
    struct st_dynamic_array *init_command;
    char *my_cnf_file,*my_cnf_group, *charset_dir, *charset_name;
    char *ssl_key;              /* PEM key file */
    char *ssl_cert;             /* PEM cert file */
    char *ssl_ca;                   /* PEM CA file */
    char *ssl_capath;               /* PEM directory of CA-s? */
    char *ssl_cipher;
    char *shared_memory_base_name;
    unsigned long max_allowed_packet;
    bool use_ssl;               /* if to use SSL or not */
    bool compress,named_pipe;
    bool reconnect, unused_1, unused_2, unused_3;
    enum mysql_option methods_to_use;
    char *bind_address;
    bool secure_auth;
    bool report_data_truncation; 
    /* function pointers for local infile support */
    int (*local_infile_init)(void **, const char *, void *);
    int (*local_infile_read)(void *, char *, unsigned int);
    void (*local_infile_end)(void *);
    int (*local_infile_error)(void *, char *, unsigned int);
    void *local_infile_userdata;
    struct st_mysql_options_extension *extension;
};

typedef struct st_net
{
    MARIADB_PVIO *pvio;
    unsigned char *buff;
    unsigned char *buff_end,*write_pos,*read_pos;
    my_socket fd;                   /* For Perl DBI/dbd */
    unsigned long remain_in_buf,length;
    unsigned long buf_length, where_b;
    unsigned long max_packet, max_packet_size;
    unsigned int pkt_nr, compress_pkt_nr;
    unsigned int write_timeout, read_timeout, retry_count;
    int fcntl;
    unsigned int *return_status;
    unsigned char reading_or_writing;
    char save_char;
    char unused_1;
    bool unused_2;
    bool compress;
    bool unused_3;
    void *unused_4;
    unsigned int last_errno;
    unsigned char error;
    bool unused_5;
    bool unused_6;
    char last_error[MYSQL_ERRMSG_SIZE];
    char sqlstate[SQLSTATE_LENGTH+1];
    struct st_mariadb_net_extension *extension;
} NET;

typedef struct st_ma_used_mem
{   /* struct for once_alloc */
    struct st_ma_used_mem *next;    /* Next block in use */
    size_t left;                 /* memory left in block  */
    size_t size;                 /* Size of block */
} MA_USED_MEM;

typedef struct st_ma_mem_root
{
    MA_USED_MEM *free;
    MA_USED_MEM *used;
    MA_USED_MEM *pre_alloc;
    size_t min_malloc;
    size_t block_size;
    unsigned int block_num;
    unsigned int first_block_usage;
    void (*error_handler)(void);
} MA_MEM_ROOT;

typedef struct st_mysql_field
{
    char *name;         /* Name of column */
    char *org_name;     /* Name of original column (added after 3.23.58) */
    char *table;            /* Table of column if column was a field */
    char *org_table;        /* Name of original table (added after 3.23.58 */
    char *db;                     /* table schema (added after 3.23.58) */
    char *catalog;                /* table catalog (added after 3.23.58) */
    char *def;          /* Default value (set by mysql_list_fields) */
    unsigned long length;       /* Width of column */
    unsigned long max_length;   /* Max width of selected set */
    /* added after 3.23.58 */
    unsigned int name_length;
    unsigned int org_name_length;
    unsigned int table_length;
    unsigned int org_table_length;
    unsigned int db_length;
    unsigned int catalog_length;
    unsigned int def_length;
    /***********************/
    unsigned int flags;     /* Div flags */
    unsigned int decimals;  /* Number of decimals in field */
    unsigned int charsetnr;       /* char set number (added in 4.1) */
    enum enum_field_types type; /* Type of field. Se mysql_com.h for types */
    void *extension;              /* added in 4.1 */
} MYSQL_FIELD;

typedef struct st_mysql
{
    NET net;            /* Communication parameters */
    void *unused_0;
    char *host,*user,*passwd,*unix_socket,*server_version,*host_info;
    char *info,*db;
    const struct ma_charset_info_st *charset;      /* character set */
    MYSQL_FIELD *fields;
    MA_MEM_ROOT field_alloc;
    unsigned long long affected_rows;
    unsigned long long insert_id;       /* id if insert on table with NEXTNR */
    unsigned long long extra_info;      /* Used by mysqlshow */
    unsigned long thread_id;        /* Id for connection in server */
    unsigned long packet_length;
    unsigned int port;
    unsigned long client_flag;
    unsigned long server_capabilities;
    unsigned int protocol_version;
    unsigned int field_count;
    unsigned int server_status;
    unsigned int server_language;
    unsigned int warning_count;          /* warning count, added in 4.1 protocol */
    struct st_mysql_options options;
    enum mysql_status status;
    bool free_me;        /* If free in mysql_close */
    bool unused_1;
    char scramble_buff[20+ 1];
    /* madded after 3.23.58 */
    bool unused_2;
    void *unused_3, *unused_4, *unused_5, *unused_6;
    LIST *stmts;
    const struct st_mariadb_methods *methods;
    void *thd;
    bool *unbuffered_fetch_owner;
    char *info_buffer;
    struct st_mariadb_extension *extension;
} MYSQL;

typedef char **MYSQL_ROW;		/* return data as array of strings */

typedef struct st_mysql_rows
{
    struct st_mysql_rows *next;		/* list of rows */
    MYSQL_ROW data;
    unsigned long length;
} MYSQL_ROWS;

typedef MYSQL_ROWS *MYSQL_ROW_OFFSET;	/* offset to current row */

typedef struct st_mysql_data
{
    MYSQL_ROWS *data;
    void *embedded_info;
    MA_MEM_ROOT alloc;
    unsigned long long rows;
    unsigned int fields;
    void *extension;
} MYSQL_DATA;

typedef struct st_mysql_res
{
    unsigned long long  row_count;
    unsigned int field_count, current_field;
    MYSQL_FIELD *fields;
    MYSQL_DATA *data;
    MYSQL_ROWS *data_cursor;
    MA_MEM_ROOT field_alloc;
    MYSQL_ROW row;			/* If unbuffered read */
    MYSQL_ROW current_row;		/* buffer to current row */
    unsigned long *lengths;		/* column lengths of current row */
    MYSQL *handle;		/* for unbuffered reads */
    bool eof;			/* Used my mysql_fetch_row */
    bool is_ps;
} MYSQL_RES;

typedef MYSQL *(__stdcall *FP_MYSQL_INIT)(
    MYSQL *mysql);

typedef unsigned long (__stdcall *FP_MYSQL_GET_CLIENT_VERSION)(
    void);

typedef unsigned long (__stdcall *FP_MYSQL_GET_SERVER_VERSION)(
    MYSQL *mysql);

typedef MYSQL * (__stdcall *FP_MYSQL_REAL_CONNECT)(
    MYSQL *mysql, 
    const char *host,
    const char *user,
    const char *passwd,
    const char *db,
    unsigned int port,
    const char *unix_socket,
    unsigned long clientflag);

typedef void (__stdcall *FP_MYSQL_CLOSE)(
    MYSQL *sock);

typedef const char * (__stdcall *FP_MYSQL_ERROR)(
    MYSQL *mysql);

typedef int (__stdcall *FP_MYSQL_SELECT_DB)(
    MYSQL *mysql, 
    const char *db);

typedef MYSQL_RES * (__stdcall *FP_MYSQL_LIST_DBS)(
    MYSQL *mysql,
    const char *wild);
    
typedef MYSQL_RES * (__stdcall *FP_MYSQL_LIST_TABLES)(
    MYSQL *mysql,
    const char *wild);
    
typedef MYSQL_RES * (__stdcall *FP_MYSQL_LIST_FIELDS)(
    MYSQL *mysql,
    const char *table,
    const char *wild);

typedef int (__stdcall *FP_MYSQL_QUERY)(
    MYSQL *mysql,
    const char *q);

typedef MYSQL_RES * (__stdcall *FP_MYSQL_STORE_RESULT)(
    MYSQL *mysql);
    
typedef void (__stdcall *FP_MYSQL_FREE_RESULT)(
    MYSQL_RES *result);

typedef unsigned int (__stdcall *FP_MYSQL_NUM_FIELDS)(
    MYSQL_RES *res);

typedef MYSQL_ROW (__stdcall *FP_MYSQL_FETCH_ROW)(
    MYSQL_RES *result);

namespace mysql
{
static FP_MYSQL_INIT init = nullptr;
static FP_MYSQL_GET_CLIENT_VERSION get_client_version = nullptr;
static FP_MYSQL_GET_SERVER_VERSION get_server_version = nullptr;
static FP_MYSQL_REAL_CONNECT real_connect = nullptr;
static FP_MYSQL_CLOSE close = nullptr;
static FP_MYSQL_ERROR error = nullptr;
static FP_MYSQL_SELECT_DB select_db = nullptr;
static FP_MYSQL_LIST_DBS list_dbs = nullptr;
static FP_MYSQL_LIST_TABLES list_tables = nullptr;
static FP_MYSQL_LIST_FIELDS list_fields = nullptr;
static FP_MYSQL_QUERY query = nullptr;
static FP_MYSQL_STORE_RESULT store_result = nullptr;
static FP_MYSQL_FREE_RESULT free_result = nullptr;
static FP_MYSQL_NUM_FIELDS num_fields = nullptr;
static FP_MYSQL_FETCH_ROW fetch_row = nullptr;
}

#endif // _MYSQL_H_