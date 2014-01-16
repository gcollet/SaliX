#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>


class PdbAtom
{
private:
	std::string _type;
	std::string _name;
	double _x;
	double _y;
	double _z;
public:
	// Constructors
	explicit PdbAtom () {}
  explicit PdbAtom (const std::string & type, const std::string & name, const double & x, const double & y, const double & z):
	_type (type), _name (name), _x (x), _y (y), _z (z) {}
	
	// Setters
	void setType (const std::string & type) {_type = type;}
	void setName (const std::string & name) {_name = name;}
	void setCoordinates (const double & x, const double & y, const double & z)
	{
		_x = x;
		_y = y;
		_z = z;
	}
	
	// Getters
	const double & x () const {return _x;}
	const double & y () const {return _x;}
	const double & z () const {return _x;}
	const std::string & name () const {return _name;}
	const std::string & type () const {return _type;}
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
};


class PdbChain : public std::vector<PdbAminoAcid>
{
private:
	char _ident;
	std::vector<PdbAminoAcid> _amino_acids;
public:
	// Constructor
	explicit PdbChain () {}
	explicit PdbChain (const char & ident): _ident (ident){}
	
	// Setters
	void SetIdent (const char & ident) {_ident = ident;}
	
	// Getters
	const char & ident () const {return _ident;}
	const std::vector<PdbAminoAcid> & amino_acids () const {return _amino_acids;}
};

class PdbModel
{
private:
	int _serial;
	std::map<char, PdbChain> _chains;
	
public:
  // Constructor
	explicit PdbModel () {}
	explicit PdbModel (const int & serial): _serial (serial) {}
	
	// Setters
	void setSerial (const int & serial) {_serial = serial;}
	void addChain (const PdbChain & chain) {_chains[chain.ident()] = chain;}
	
	// Getters
	const int & serial () const {return _serial;}
	const PdbChain & chain (const char & ident) const {return _chains.at(ident);}
	const std::map<char, PdbChain> & chains () const {return _chains;}
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
		
	}
	
	// Setters
	void addModel (const PdbModel & model) {_models[model.serial()] = model;}
	
	// Getters
	const PdbModel & model (const int & serial) const {return _models.at(serial);}
	const std::map<int, PdbModel> & models () const {return _models;}
};

int main(int argc, const char * argv[])
{
	const clock_t begin_time = clock();
	
	std::string app_name = argv[0];
	app_name = app_name.substr(app_name.rfind("/") + 1);
	if (argc != 3) {
		std::cerr << "Usage: " << app_name << " <pdb_file1> <pdb_file2>\n";
		return 1;
	}
	
	PdbStructure pdb_structure1 (argv[1]);
	PdbStructure pdb_structure2 (argv[2]);
	
	std::cerr << app_name << " executed in " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " seconds \n";
	return 0;
}
