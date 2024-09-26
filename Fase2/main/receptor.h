#ifndef RECEPTOR_H
#define RECEPTOR_H

#include <string>

class Receptor {
private:
    std::string emisor;
    std::string receptor;
    std::string estado;

public:
    Receptor(const std::string& emisor_, const std::string& receptor_, const std::string& estado_);

    // Getters
    std::string getEmisor() const;
    std::string getReceptor() const;
    std::string getEstado() const;

    // Setters
    void setEmisor(const std::string& emisor_);
    void setReceptor(const std::string& receptor_);
    void setEstado(const std::string& estado_);
};

#endif // RECEPTOR_H
