#include <M5Unified.h>

enum class Page {Accueil, Reglage, Debug, Profil, Etat};
Page Page_actuelle = Page::Accueil;

struct Icone
    {
        int x, y, w, h;
        const char* nom;
        uint16_t couleur_contour;  
    };

Icone Reglage = {20, 70, 130, 40, "Reglage", TFT_GREEN};
Icone Profil  = {170, 70, 130, 40, "Profil", TFT_GREEN};
Icone Debug   = {20, 140, 130, 40, "Debug", TFT_GREEN};
Icone Etat    = {170, 140, 130, 40, "Etat", TFT_GREEN};
Icone Retour  = {10,  185, 100, 45, "Retour", TFT_GREEN};


void dessiner_Icone (const Icone& ic)
    {
        M5.Display.drawRoundRect(ic.x, ic.y, ic.w, ic.h, 12, ic.couleur_contour);
        M5.Display.setTextSize(2);
        M5.Display.setTextDatum(middle_center);
        M5.Display.drawString(ic.nom, ic.x + ic.w/2, ic.y + ic.h/2);
    }

bool est_touche(const Icone& ic, int touche_x, int touche_y)
    {
        return touche_x >= ic.x && touche_x < ic.x + ic.w &&
               touche_y >= ic.y && touche_y < ic.y + ic.h;
    }

void titre(const char* texte)
    {
        M5.Display.setTextSize(2);
        M5.Display.setTextDatum(top_center);
        M5.Display.drawString(texte, 160, 20);
    }

void Changer_page()
    {
        M5.Display.fillScreen(TFT_BLACK);
    
        switch (Page_actuelle)
            {
                case Page::Accueil:
                    titre("Accueil");
                    dessiner_Icone(Reglage);
                    dessiner_Icone(Debug);
                    dessiner_Icone(Profil);
                    dessiner_Icone(Etat);
                    break;

                case Page::Reglage:
                    titre("Reglage");
                    dessiner_Icone(Retour);
                    break;

                case Page::Debug:
                    titre("Debug");
                    dessiner_Icone(Retour);
                    break;

                case Page::Profil:
                    titre("Profil");
                    dessiner_Icone(Retour);
                    break;

                case Page::Etat:
                    titre("Etat");
                    dessiner_Icone(Retour);
                    break;

            }
    }

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);    
  Changer_page();     
}

void loop()
    {
        M5.update();
        auto touche = M5.Touch.getDetail();

        if (!touche.wasClicked()) return;

        if (Page_actuelle == Page::Accueil)
            {
                if      (est_touche(Reglage, touche.x, touche.y)) Page_actuelle = Page::Reglage;
                else if (est_touche(Profil,  touche.x, touche.y)) Page_actuelle = Page::Profil;
                else if (est_touche(Debug,   touche.x, touche.y)) Page_actuelle = Page::Debug;
                else if (est_touche(Etat,    touche.x, touche.y)) Page_actuelle = Page::Etat;
                else return;
            }

        else
            {
                if      (est_touche(Retour, touche.x, touche.y)) Page_actuelle = Page::Accueil;
                else return;
            }

        Changer_page();
    }