/* wuav.c generated by valac, the Vala compiler
 * generated from wuav.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gio/gio.h>
#include <libsoup/soup.h>


#define TYPE_MAIN (main_get_type ())
#define MAIN(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_MAIN, Main))
#define MAIN_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_MAIN, MainClass))
#define IS_MAIN(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_MAIN))
#define IS_MAIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_MAIN))
#define MAIN_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_MAIN, MainClass))

typedef struct _Main Main;
typedef struct _MainClass MainClass;
typedef struct _MainPrivate MainPrivate;
#define _g_option_context_free0(var) ((var == NULL) ? NULL : (var = (g_option_context_free (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_string_free0(var) ((var == NULL) ? NULL : (var = (g_string_free (var, TRUE), NULL)))

typedef enum  {
	WUAV_ERROR_LOGIN
} WuavError;
#define WUAV_ERROR wuav_error_quark ()
struct _Main {
	GObject parent_instance;
	MainPrivate * priv;
};

struct _MainClass {
	GObjectClass parent_class;
};


static char* main_username;
static char* main_username = NULL;
static char* main_password;
static char* main_password = NULL;
static gboolean main_show_available;
static gboolean main_show_available = FALSE;
static gpointer main_parent_class = NULL;

GQuark wuav_error_quark (void);
GType main_get_type (void);
enum  {
	MAIN_DUMMY_PROPERTY
};
static void main_login (const char* username, const char* password, SoupSession* session, GError** error);
static char* main_search_album (const char* artist, const char* album, SoupSession* session);
static gint main_main (char** args, int args_length1);
Main* main_new (void);
Main* main_construct (GType object_type);
static void main_finalize (GObject* obj);
static void _vala_array_destroy (gpointer array, gint array_length, GDestroyNotify destroy_func);
static void _vala_array_free (gpointer array, gint array_length, GDestroyNotify destroy_func);
static gint _vala_array_length (gpointer array);

static const char* MAIN_audio_formats_site[6] = {"FLAC / Lossless", "MP3 / 320", "MP3 / V0", "MP3 / V2", "Ogg / q8.x", "AAC / 320"};
static const char* MAIN_audio_formats_printable[6] = {"FLAC", "320", "V0", "V2", "OGG", "AAC"};
static const GOptionEntry MAIN_options[4] = {{"username", 'u', 0, G_OPTION_ARG_STRING, &main_username, "Your What.CD username", NULL}, {"password", 'p', 0, G_OPTION_ARG_STRING, &main_password, "Your What.CD password", NULL}, {"show-available", 'a', 0, G_OPTION_ARG_NONE, &main_show_available, "List available formats. The default is to list formats that are not av" \
"ailable.", NULL}, {NULL}};


GQuark wuav_error_quark (void) {
	return g_quark_from_static_string ("wuav_error-quark");
}


static gint main_main (char** args, int args_length1) {
	gint result = 0;
	GError * _inner_error_;
	gboolean _tmp0_ = FALSE;
	gboolean _tmp1_ = FALSE;
	GFile* file;
	SoupSessionAsync* session;
	SoupCookieJar* cookies;
	_inner_error_ = NULL;
	{
		GOptionContext* opt;
		opt = g_option_context_new ("<album-list-file>");
		g_option_context_add_main_entries (opt, MAIN_options, NULL);
		g_option_context_parse (opt, &args_length1, &args, &_inner_error_);
		if (_inner_error_ != NULL) {
			_g_option_context_free0 (opt);
			goto __catch0_g_error;
		}
		_g_option_context_free0 (opt);
	}
	goto __finally0;
	__catch0_g_error:
	{
		GError * e;
		e = _inner_error_;
		_inner_error_ = NULL;
		{
			fprintf (stderr, "Error: %s\n", e->message);
			fprintf (stderr, "Run '%s --help' to see a full list of available " "options\n", args[0]);
			result = 1;
			_g_error_free0 (e);
			return result;
		}
	}
	__finally0:
	if (_inner_error_ != NULL) {
		g_critical ("file %s: line %d: uncaught error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
		g_clear_error (&_inner_error_);
		return 0;
	}
	if (args_length1 != 2) {
		_tmp1_ = TRUE;
	} else {
		_tmp1_ = main_username == NULL;
	}
	if (_tmp1_) {
		_tmp0_ = TRUE;
	} else {
		_tmp0_ = main_password == NULL;
	}
	if (_tmp0_) {
		fprintf (stderr, "Run '%s --help' for usage\n", args[0]);
		result = 1;
		return result;
	}
	file = g_file_new_for_commandline_arg (args[1]);
	if (!g_file_query_exists (file, NULL)) {
		char* _tmp2_;
		fprintf (stderr, "File '%s' does not exist\n", _tmp2_ = g_file_get_path (file));
		_g_free0 (_tmp2_);
		result = 1;
		_g_object_unref0 (file);
		return result;
	}
	session = (SoupSessionAsync*) soup_session_async_new ();
	g_object_set ((SoupSession*) session, "user-agent", "wuav", NULL);
	cookies = soup_cookie_jar_new ();
	soup_session_feature_attach ((SoupSessionFeature*) cookies, (SoupSession*) session);
	{
		GFileInputStream* _tmp3_;
		GFileInputStream* _tmp4_;
		GDataInputStream* _tmp5_;
		GDataInputStream* in_stream;
		char* line;
		main_login (main_username, main_password, (SoupSession*) session, &_inner_error_);
		if (_inner_error_ != NULL) {
			goto __catch1_g_error;
		}
		_tmp3_ = g_file_read (file, NULL, &_inner_error_);
		if (_inner_error_ != NULL) {
			goto __catch1_g_error;
		}
		in_stream = (_tmp5_ = g_data_input_stream_new ((GInputStream*) (_tmp4_ = _tmp3_)), _g_object_unref0 (_tmp4_), _tmp5_);
		line = NULL;
		while (TRUE) {
			char* _tmp6_;
			char* _tmp7_;
			char** _tmp9_;
			gint _vals_size_;
			gint vals_length1;
			char** _tmp8_;
			char** vals;
			char* artist;
			char* album;
			char* _tmp10_;
			_tmp6_ = g_data_input_stream_read_line (in_stream, NULL, NULL, &_inner_error_);
			if (_inner_error_ != NULL) {
				_g_object_unref0 (in_stream);
				_g_free0 (line);
				goto __catch1_g_error;
			}
			if (!((line = (_tmp7_ = _tmp6_, _g_free0 (line), _tmp7_)) != NULL)) {
				break;
			}
			vals = (_tmp9_ = _tmp8_ = g_strsplit (line, "/", 0), vals_length1 = _vala_array_length (_tmp8_), _vals_size_ = vals_length1, _tmp9_);
			artist = g_strdup (vals[0]);
			album = g_strdup (vals[1]);
			fprintf (stdout, "%s %s - %s\n", _tmp10_ = main_search_album (artist, album, (SoupSession*) session), artist, album);
			_g_free0 (_tmp10_);
			vals = (_vala_array_free (vals, vals_length1, (GDestroyNotify) g_free), NULL);
			_g_free0 (artist);
			_g_free0 (album);
		}
		_g_object_unref0 (in_stream);
		_g_free0 (line);
	}
	goto __finally1;
	__catch1_g_error:
	{
		GError * e;
		e = _inner_error_;
		_inner_error_ = NULL;
		{
			fprintf (stderr, "Error: %s\n", e->message);
			result = 1;
			_g_error_free0 (e);
			_g_object_unref0 (file);
			_g_object_unref0 (session);
			_g_object_unref0 (cookies);
			return result;
		}
	}
	__finally1:
	if (_inner_error_ != NULL) {
		_g_object_unref0 (file);
		_g_object_unref0 (session);
		_g_object_unref0 (cookies);
		g_critical ("file %s: line %d: uncaught error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
		g_clear_error (&_inner_error_);
		return 0;
	}
	result = 0;
	_g_object_unref0 (file);
	_g_object_unref0 (session);
	_g_object_unref0 (cookies);
	return result;
}


int main (int argc, char ** argv) {
	g_type_init ();
	return main_main (argv, argc);
}


static gpointer _g_object_ref0 (gpointer self) {
	return self ? g_object_ref (self) : NULL;
}


static gboolean string_contains (const char* self, const char* needle) {
	gboolean result = FALSE;
	g_return_val_if_fail (self != NULL, FALSE);
	g_return_val_if_fail (needle != NULL, FALSE);
	result = strstr (self, needle) != NULL;
	return result;
}


static void main_login (const char* username, const char* password, SoupSession* session, GError** error) {
	GError * _inner_error_;
	SoupMessage* login_message;
	g_return_if_fail (username != NULL);
	g_return_if_fail (password != NULL);
	g_return_if_fail (session != NULL);
	_inner_error_ = NULL;
	login_message = _g_object_ref0 (soup_form_request_new ("POST", "https://ssl.what.cd/login.php", "username", username, "password", password, "keeplogged", "0", NULL, NULL));
	soup_session_send_message (session, login_message);
	if (string_contains ((const char*) login_message->response_body->data, "id=\"loginform\"")) {
		_inner_error_ = g_error_new_literal (WUAV_ERROR, WUAV_ERROR_LOGIN, "Login failed");
		{
			if (_inner_error_->domain == WUAV_ERROR) {
				g_propagate_error (error, _inner_error_);
				_g_object_unref0 (login_message);
				return;
			} else {
				_g_object_unref0 (login_message);
				g_critical ("file %s: line %d: uncaught error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
				g_clear_error (&_inner_error_);
				return;
			}
		}
	}
	_g_object_unref0 (login_message);
}


static glong string_get_length (const char* self) {
	glong result;
	g_return_val_if_fail (self != NULL, 0L);
	result = g_utf8_strlen (self, -1);
	return result;
}


static char* main_search_album (const char* artist, const char* album, SoupSession* session) {
	char* result = NULL;
	GString* retval;
	SoupMessage* search_message;
	g_return_val_if_fail (artist != NULL, NULL);
	g_return_val_if_fail (album != NULL, NULL);
	g_return_val_if_fail (session != NULL, NULL);
	retval = g_string_new ("");
	search_message = _g_object_ref0 (soup_form_request_new ("GET", "http://ssl.what.cd/torrents.php", "artistname", artist, "groupname", album, "advanced", NULL, NULL));
	soup_session_send_message (session, search_message);
	{
		gint i;
		i = 0;
		{
			gboolean _tmp0_;
			_tmp0_ = TRUE;
			while (TRUE) {
				char* s;
				gboolean exists;
				char* _tmp2_;
				if (!_tmp0_) {
					i = i + 1;
				}
				_tmp0_ = FALSE;
				if (!(i < G_N_ELEMENTS (MAIN_audio_formats_site))) {
					break;
				}
				s = g_strdup ("");
				exists = string_contains ((const char*) search_message->response_body->data, MAIN_audio_formats_site[i]);
				if (exists == main_show_available) {
					char* _tmp1_;
					s = (_tmp1_ = g_strdup (MAIN_audio_formats_printable[i]), _g_free0 (s), _tmp1_);
				}
				g_string_append (retval, _tmp2_ = g_strdup_printf ("%*s ", (gint) string_get_length (MAIN_audio_formats_printable[i]), s));
				_g_free0 (_tmp2_);
				_g_free0 (s);
			}
		}
	}
	result = g_strdup (retval->str);
	_g_string_free0 (retval);
	_g_object_unref0 (search_message);
	return result;
}


Main* main_construct (GType object_type) {
	Main * self;
	self = (Main*) g_object_new (object_type, NULL);
	return self;
}


Main* main_new (void) {
	return main_construct (TYPE_MAIN);
}


static void main_class_init (MainClass * klass) {
	main_parent_class = g_type_class_peek_parent (klass);
	G_OBJECT_CLASS (klass)->finalize = main_finalize;
}


static void main_instance_init (Main * self) {
}


static void main_finalize (GObject* obj) {
	Main * self;
	self = MAIN (obj);
	G_OBJECT_CLASS (main_parent_class)->finalize (obj);
}


GType main_get_type (void) {
	static volatile gsize main_type_id__volatile = 0;
	if (g_once_init_enter (&main_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (MainClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) main_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (Main), 0, (GInstanceInitFunc) main_instance_init, NULL };
		GType main_type_id;
		main_type_id = g_type_register_static (G_TYPE_OBJECT, "Main", &g_define_type_info, 0);
		g_once_init_leave (&main_type_id__volatile, main_type_id);
	}
	return main_type_id__volatile;
}


static void _vala_array_destroy (gpointer array, gint array_length, GDestroyNotify destroy_func) {
	if ((array != NULL) && (destroy_func != NULL)) {
		int i;
		for (i = 0; i < array_length; i = i + 1) {
			if (((gpointer*) array)[i] != NULL) {
				destroy_func (((gpointer*) array)[i]);
			}
		}
	}
}


static void _vala_array_free (gpointer array, gint array_length, GDestroyNotify destroy_func) {
	_vala_array_destroy (array, array_length, destroy_func);
	g_free (array);
}


static gint _vala_array_length (gpointer array) {
	int length;
	length = 0;
	if (array) {
		while (((gpointer*) array)[length]) {
			length++;
		}
	}
	return length;
}




