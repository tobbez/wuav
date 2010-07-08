using GLib;
using Soup;

errordomain WuavError {
	LOGIN
}

public class Main : GLib.Object
{
	static string username;
	static string password;
	static bool show_available = false;

	static const string audio_formats_site [] = { "FLAC / Lossless", "MP3 / 320", "MP3 / V0", "MP3 / V2", "Ogg / q8.x", "AAC / 320" };
	static const string audio_formats_printable [] = { "FLAC", "320", "V0", "V2", "OGG", "AAC" };

	const OptionEntry[] options = {
		{ "username", 'u', 0, OptionArg.STRING, ref username, "Your What.CD username", null },
		{ "password", 'p', 0, OptionArg.STRING, ref password, "Your What.CD password", null },
		{ "show-available", 'a', 0, OptionArg.NONE, ref show_available, "List available formats. The default is to list formats that are not available.", null},
		{ null }
	};

	static int main (string[] args)
	{
		try {
			var opt = new OptionContext ("<album-list-file>");
			opt.add_main_entries (options, null);
			opt.parse (ref args);
		} catch (GLib.Error e) {
			stderr.printf ("Error: %s\n", e.message);
			stderr.printf ("Run '%s --help' to see a full list of available "+
						   "options\n", args[0]); 
			return 1;
		}

		if (args.length != 2 || username == null || password == null) {
			stderr.printf ("Run '%s --help' for usage\n", args[0]);
			return 1;
		}

		var file = File.new_for_commandline_arg (args[1]);
		if (!file.query_exists (null)) {
			stderr.printf ("File '%s' does not exist\n", file.get_path ());
		}


		var session = new Soup.SessionAsync ();
		session.user_agent = "wuav";

		var cookies = new Soup.CookieJar ();
		cookies.attach (session);


		try {
			login (username, password, session);

			var in_stream = new DataInputStream (file.read (null));
			string line;
			while ((line = in_stream.read_line (null, null)) != null) {
				var vals = line.split ("/");
				string artist = vals[0], album = vals[1];
				stdout.printf ("%s %s - %s\n", 
							   search_album (artist, album, session),
							   artist, album);
			}
		} catch (GLib.Error e) {
			stderr.printf ("Error: %s\n", e.message);
			return 1;
		}

		return 0;
	}

	static void login (string username, string password, Soup.Session session)
		throws WuavError
	{
		var login_message = 
			Soup.form_request_new ("POST", "https://ssl.what.cd/login.php",
								   "username", username,
								   "password", password,
								   "keeplogged", "0",
								   null);

		session.send_message (login_message);
		if (((string)login_message.response_body.data).contains ("id=\"loginform\"")) {
			throw new WuavError.LOGIN ("Login failed");
		}
	}

	static string search_album (string artist, string album, Soup.Session session)
	{
		var retval = new StringBuilder ();
		var search_message = 
			Soup.form_request_new ("GET", "http://ssl.what.cd/torrents.php",
								   "artistname", artist,
								   "groupname", album,
								   "advanced",
								   null);

		session.send_message (search_message);

		for (var i = 0; i < audio_formats_site.length; ++i) {
			var s = "";
			bool exists = ((string)search_message.response_body.data).contains (audio_formats_site[i]);
			if (exists == show_available) {
				s = audio_formats_printable[i];
			}
			retval.append("%*s ".printf((int)audio_formats_printable[i].length, s));
		}

		return retval.str;
	}
}
