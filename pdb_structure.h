class PdbAtom
{
private:
	std::string _name;
	double _x;
	double _y;
	double _z;
public:
	// Constructors
	explicit PdbAtom () {}
	explicit PdbAtom (const std::string & name, const double & x, const double & y, const double & z):
	_name (name), _x (x), _y (y), _z (z) {}
	
	// Setters
	void setName (const std::string & name) {_name = name;}
	void setCoordinates (const double & x, const double & y, const double & z)
	{
		_x = x;
		_y = y;
		_z = z;
	}
	
	// Getters
	const std::string & name () const {return _name;}
	const double & x () const {return _x;}
	const double & y () const {return _x;}
	const double & z () const {return _x;}
	
	void print() {
		std::cout << _name << "\t" << _x << "\t" << _y << "\t" << _z << "\n";
	}
};


class PdbAminoAcid
{
private:
	std::string _index;
	std::string _name;
	std::map<std::string, PdbAtom> _backbone;
	std::map<std::string, PdbAtom> _sidechain;
public:
	// Constructors
	explicit PdbAminoAcid () {}
	explicit PdbAminoAcid (const std::string & index, const std::string & name): _index (index), _name (name) {}
	
	void clear ()
	{
		_index.clear();
		_name.clear();
		_backbone.clear();
		_sidechain.clear();
	}
	
	// Setters
	void setIndex (const std::string & index) {_index = index;}
	void setName (const std::string & name) {_name = name;}
	void addBackboneAtom (const PdbAtom & atom) {_backbone[atom.name()] = atom;}
	void addSidechainAtom (const PdbAtom & atom) {_sidechain[atom.name()] = atom;}
	
	// Getters
	const std::string name () const {return _name;}
	const std::string index () const {return _index;}
	const PdbAtom & backboneAtom (const std::string name) const {return _backbone.at(name);}
	const PdbAtom & sidechainAtom (const std::string name) const {return _sidechain.at(name);}
	const	std::map<std::string, PdbAtom> & backbone () const {return _backbone;}
	const	std::map<std::string, PdbAtom> & sidechain () const {return _sidechain;}
	
	void print() {
		for (std::map<std::string, PdbAtom>::iterator it = _backbone.begin(); it != _backbone.end(); it++) {
			std::cout << _name << "\t" << _index << "\t";
			it->second.print();
		}
	}
};


class PdbChain : public std::vector<PdbAminoAcid>
{
private:
	char _ident;
public:
	// Constructor
	explicit PdbChain () {}
	explicit PdbChain (const char & ident): _ident (ident){}
	
	// Setters
	void SetIdent (const char & ident) {_ident = ident;}
	
	// Getters
	const char & ident () const {return _ident;}
	
	void print() {
		for (std::vector<PdbAminoAcid>::iterator it = begin(); it != end(); it++) {
			(*it).print();
		}
	}
};

class PdbModel
{
private:
	int _serial;
	std::map<char, PdbChain> _chains;
	
public:
	// Constructor
	explicit PdbModel ()
	{
		_serial = -1; // unset flag
	}
	explicit PdbModel (const int & serial): _serial (serial) {}
	
	void clear() {
		_chains.clear();
		_serial = -1;
	}
	
	// Setters
	void setSerial (const int & serial) {_serial = serial;}
	void addChain (const PdbChain & chain) {_chains[chain.ident()] = chain;}
	
	// Getters
	const int & serial () const {return _serial;}
	const PdbChain & chain (const char & ident) const {return _chains.at(ident);}
	const std::map<char, PdbChain> & chains () const {return _chains;}
	
	void print() {
		for (std::map<char, PdbChain>::iterator it = _chains.begin(); it != _chains.end(); it++) {
			it->second.print();
		}
	}
};

class PdbStructure
{
private:
	std::string _file_name;
	std::map<int, PdbModel> _models;
	
public:
	// Constructor
	explicit PdbStructure (const std::string & file_name): _file_name (file_name)
	{
		PdbChain current_chain;
		PdbModel current_model;
		std::ifstream infile;
		infile.open (_file_name.c_str());
		if (!infile.good()) {
			throw std::runtime_error("Cannot open file");
		}
		// Define pdb fields
		std::string atom_field   = "ATOM  ";
		std::string model_field  = "MODEL ";
		std::string endmdl_field = "ENDMDL";
		std::string ter_field    = "TER   ";
		
		while (infile.good()) {
			std::string line;
			getline (infile, line);
			if (line.find(atom_field) == 0) {
				std::string atom_name = line.substr(12, 4);
				if (atom_name.compare(" CA ") == 0) {
					char chain_id = line[21];
					PdbAtom current_atom (atom_name, atof(line.substr(30, 8).c_str()), atof(line.substr(38, 8).c_str()), atof(line.substr(46, 8).c_str()));
					if (current_chain.ident() != chain_id) {
						current_chain.SetIdent(chain_id);
					}
					PdbAminoAcid current_aa (line.substr(22,5), line.substr(17, 3));
					current_aa.addBackboneAtom(current_atom);
					current_chain.push_back(current_aa);
				}
			} else if (line.find(model_field) == 0) {
				current_model.clear();
				current_model.setSerial(atoi(line.substr(10, 4).c_str()));
			} else if (line.find(endmdl_field) == 0) {
				_models[current_model.serial()] = current_model;
			} else if (line.find(ter_field) == 0) {
				current_model.addChain(current_chain);
				current_chain.clear();
			}
		}
		if (current_model.serial() == -1) {
			_models[current_model.serial()] = current_model;
		}
		infile.close();
	}
	
	// Setters
	void addModel (const PdbModel & model) {_models[model.serial()] = model;}
	
	// Getters
	const PdbModel & model (const int & serial) const {return _models.at(serial);}
	const std::map<int, PdbModel> & models () const {return _models;}
	
	void print() {
		for (std::map<int, PdbModel>::iterator it = _models.begin(); it != _models.end(); it++) {
			it->second.print();
		}
	}
};
