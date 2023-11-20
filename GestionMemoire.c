#include "./libs/lib.h"
#include <stdio.h>

const int TAILLE_PAGE = 1024;

unsigned int calculerNumeroDePage(unsigned long adresse) {
	// TODO
    return adresse / TAILLE_PAGE;
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
	// TODO
	return adresse % TAILLE_PAGE;
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
	// TODO
     return numeroDePage * TAILLE_PAGE + deplacementDansLaPage;
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
	req->adressePhysique = 0;
	for(int i = 0; i < TAILLE_TLB; i++){
		if(mem->tlb->numeroPage[i] == calculerNumeroDePage(req->adresseVirtuelle) && mem->tlb->entreeValide[i]){
			int numeroDeCadre = mem->tlb->numeroCadre[i];
			req->adressePhysique = calculerAdresseComplete(numeroDeCadre, calculerDeplacementDansLaPage(req->adresseVirtuelle));
			mem->tlb->dernierAcces[i] = req->date;
			break;
		}
		
	}
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
	req->adressePhysique = 0;
	unsigned int numeroDePage = calculerNumeroDePage(req->adresseVirtuelle);
	if(mem->tp->entreeValide[numeroDePage]){
		int numeroDeCadre = mem->tp->numeroCadre[numeroDePage];
		req->adressePhysique = calculerAdresseComplete(numeroDeCadre, calculerDeplacementDansLaPage(req->adresseVirtuelle));
	}
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	//TODO
	for(int i = 0; i < TAILLE_MEMOIRE; i++){
		int numeroDePage = calculerNumeroDePage(req->adresseVirtuelle);
		if(!mem->memoire->utilisee[i]){
			mem->memoire->numeroPage[i] = numeroDePage;
			mem->memoire->dateCreation[i] = req->date;
			mem->memoire->dernierAcces[i] = req->date;
			mem->memoire->utilisee[i] = 1;
			req->adressePhysique = calculerAdresseComplete(i, calculerDeplacementDansLaPage(req->adresseVirtuelle));
			break;
		}
	}
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	// TODO
	int entreeARemplacer = 0;
	for(int i = 0; i < TAILLE_TLB; i++){
		if(mem->tlb->entreeValide[i] == 0) {
			entreeARemplacer = i;
			break;
		}

		if(mem->tlb->dateCreation[i] < mem->tlb->dateCreation[entreeARemplacer]){
			entreeARemplacer = i;
		}
	}
	mem->tlb->numeroPage[entreeARemplacer] = calculerNumeroDePage(req->adresseVirtuelle);
	mem->tlb->dateCreation[entreeARemplacer] = req->date;
	mem->tlb->dernierAcces[entreeARemplacer] = req->date;
	mem->tlb->numeroCadre[entreeARemplacer] = calculerNumeroDePage(req->adressePhysique);
	mem->tlb->entreeValide[entreeARemplacer] = 1;
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
