# 🎮 SuperNils — Règles du jeu

Bienvenue dans **SuperNils** !

Dans ce jeu, tu contrôles **Nils**, un héros qui doit défendre l’ISEN contre une invasion de monstres.  
Ta mission : survivre le plus longtemps possible en éliminant les monstres qui apparaissent en **tirant des lasers avec tes yeux** (comme Superman).

---

## 🕹️ Comment jouer

- **Déplacement** :  
  Utilise les touches **Z Q S D** pour déplacer Nils.

- **Changer d’arme** :
  - Appuie sur **X** pour utiliser le **pistolet laser** (tir rapide, un coup par tir).
  - Appuie sur **C** pour utiliser le **shotgun laser** (tir plus puissant, plusieurs balles mais plus lent).

- **Commencer la partie** :
  - Clique sur le bouton **Lancer le jeu**.
  - Clique ensuite sur **Nils** (le personnage) pour pouvoir le déplacer.

---

## 👾 Les monstres

Quatre types de monstres attaquent l’ISEN :

| Monstre    | Comportement                        | Effet sur Nils                                                                        | Points gagnés | Points perdus |
|------------|-------------------------------------|----------------------------------------------------------------------------------------|---------------|----------------|
| 🧪 **Doctor**   | Tire des boules de feu à distance | Attaque à distance                                                                    | +400          | Oui             |
| 💧 **Slime**    | Attaque au corps à corps         | Ralentit Nils + dégâts                                                                | +250          | Oui             |
| 👻 **Ghost**    | Traverse les murs                | Touche = impossibilité de tirer pendant quelques secondes                             | +300          | Oui             |
| 🐦 **Corbeau**  | Fonce en mêlée                   | Dégâts au corps à corps                                                               | +250          | Oui             |

---

## 📈 Score

- Chaque monstre tué te rapporte des points.
- Si un monstre te touche, tu **perds** des points.
- Le score s’affiche en **bas à droite** de l’écran.
- Objectif : **atteindre le meilleur score possible** !

---

## ❤️ Barre de vie

- La barre de vie de Nils est affichée en bas à droite.
- Elle diminue à chaque contact avec un monstre.
- Si elle tombe à zéro, la partie est terminée.

---

# 🛠️ SuperNils — Guide de compilation et d’exécution

Le jeu utilise **Qt6** avec les modules suivants :
- `Qt Widgets`
- `Qt Multimedia`
- `Qt Core`, `Qt Gui`

### 📂 Contenu fourni
- Le code source (`.cpp`, `.hpp`)
- Le fichier `CMakeLists.txt` prêt à l’emploi
- Les ressources (images, sons, sprites)

---

## 🧰 Prérequis
- Qt 6 installé (Widgets, Multimedia, etc.)
- CMake ≥ 3.16
- Un compilateur (g++ / Clang / MSVC)
- Un IDE : CLion, Qt Creator, ou VSCode avec l’extension **CMake Tools**

---

## ⚙️ Compilation avec CLion ou Qt Creator

1. Ouvrez le dossier du projet contenant le `CMakeLists.txt`.
2. L’IDE détecte Qt automatiquement (ou configurez manuellement le chemin Qt).
3. Cochez 'Reload CMake Project on editing CMakeLists.txt' si disponible.
4. Cliquez sur **Build** puis sur **Run**.

---

## 🚀 Lancer le jeu

Une fois la compilation terminée, plusieurs options :

- ▶️ **Depuis l’IDE** : bouton **Run**
- 💻 **Terminal** :
  ```bash
  ./SuperNils      # Linux/macOS
  SuperNils.exe    # Windows
