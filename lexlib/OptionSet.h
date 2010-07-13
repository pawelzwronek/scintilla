// Scintilla source code edit control
/** @file OptionSet.h
 ** Manage descriptive information about an options struct for a lexer.
 ** Hold the names, positions, and descriptions of boolean, integer and string options and
 ** allow setting options and retrieving metadata about the options.
 **/
// Copyright 2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef OPTIONSET_H
#define OPTIONSET_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

template <typename T>
class OptionSet {
	typedef T Target;
	typedef bool T::*plcob;
	typedef int T::*plcoi;
	typedef std::string T::*plcos;
	struct Option {
		int opType;
		union {
			plcob pb;
			plcoi pi;
			plcos ps;
		};
		std::string description;
		Option(plcob pb_=0, std::string description_="") :
			opType(SC_TYPE_BOOLEAN), pb(pb_), description(description_) {
		}
		Option(plcoi pi_, std::string description_) :
			opType(SC_TYPE_INTEGER), pi(pi_), description(description_) {
		}
		Option(plcos ps_, std::string description_) :
			opType(SC_TYPE_STRING), ps(ps_), description(description_) {
		}
		bool Set(T *base, const char *val) {
			switch (opType) {
			case SC_TYPE_BOOLEAN: {
					bool option = atoi(val) != 0;
					if ((*base).*pb != option) {
						(*base).*pb = option;
						return true;
					}
					break;
				}
			case SC_TYPE_INTEGER: {
					int option = atoi(val);
					if ((*base).*pi != option) {
						(*base).*pi = option;
						return true;
					}
					break;
				}
			case SC_TYPE_STRING: {
					if ((*base).*ps != val) {
						(*base).*ps = val;
						return true;
					}
					break;
				}
			}
			return false;
		}
	};
	typedef std::map<std::string, Option> OptionMap;
	OptionMap nameToDef;
	std::string names;
	std::string wordLists;

	void AppendName(const char *name) {
		if (!names.empty())
			names += "\n";
		names += name;
	}
public:
	void DefineProperty(const char *name, plcob pb, std::string description="") {
		nameToDef[name] = Option(pb, description);
		AppendName(name);
	}
	void DefineProperty(const char *name, plcoi pi, std::string description="") {
		nameToDef[name] = Option(pi, description);
		AppendName(name);
	}
	void DefineProperty(const char *name, plcos ps, std::string description="") {
		nameToDef[name] = Option(ps, description);
		AppendName(name);
	}
	const char *PropertyNames() {
		return names.c_str();
	}
	int PropertyType(const char *name) {
		OptionMap::iterator it = nameToDef.find(name);
		if (it != nameToDef.end()) {
			return it->second.opType;
		}
		return SC_TYPE_BOOLEAN;
	}
	const char *DescribeProperty(const char *name) {
		OptionMap::iterator it = nameToDef.find(name);
		if (it != nameToDef.end()) {
			return it->second.description.c_str();
		}
		return "";
	}

	bool PropertySet(T *base, const char *name, const char *val) {
		OptionMap::iterator it = nameToDef.find(name);
		if (it != nameToDef.end()) {
			return it->second.Set(base, val);
		}
		return false;
	}

	void DefineWordListSets(const char * const wordListDescriptions[]) {
		if (wordListDescriptions) {
			for (size_t wl = 0; wordListDescriptions[wl]; wl++) {
				if (!wordLists.empty())
					wordLists += "\n";
				wordLists += wordListDescriptions[wl];
			}
		}
	}

	const char *DescribeWordListSets() {
		return wordLists.c_str();
	}
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
