PRIMO:  oscillazione_eta.c (che necessita di oscillazione.h, input.txt, valori_n.txt OPPURE valori_eta.txt a seconda della simulazione che si vuole effettuare con somma gioia)
SECONDO:  bootstrap_eta-N.c (che necessita di input.txt, valori_n.txt OPPURE valori_eta.txt a seconda della simulazione che si vuole effettuare con somma gioia)
TERZO:  potenziale.py (pesca da caartella N_ETA_fisso, riempita da simulazione 1, con dentro cartelle N_Eta=... (tipo 20,30,40,50) e dentro ognuna vi è cartella Bootstrap)
QUARTO:  cinetico.py (  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /  / /)
QUINTO:  energia_totale.py (devo runnare simulazione 0 con eta piccolo (tipo 0.01 o 0.005) e pesca da cartella N_variabile/Bootstrap)
SESTO:  funzione d'onda del fondamentale -> da finire di implementare+
SETTIMO:  nell'ordine  oscillazione_eta_splitting.c -> Ck_errors.py -> ck_bootstrap.c -> bootCk.py -> splitting_view.py


NOTA BENISSIMO:  runna almeno con UN MILIONE di punti in input e idec 1 (se fa schifo si aumenta) e iterm 1000000