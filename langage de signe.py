# Importation des bibliothèques nécessaires
import cv2  # Pour la capture vidéo et le traitement d'images
import cvzone  # Pour des fonctionnalités supplémentaires comme l'affichage de texte
from cvzone.HandTrackingModule import HandDetector  # Pour détecter les mains
import os  # Pour gérer les fichiers temporaires
import pygame  # Pour la lecture audio
from gtts import gTTS  # Pour la synthèse vocale

# Initialisation de pygame pour la lecture audio
pygame.mixer.init()

# Initialisation du détecteur de mains
Hand_Detector = HandDetector(detectionCon=0.8, maxHands=1)  # Détecter une seule main avec une confiance de 80%

# Ouverture de la caméra (0 est généralement la caméra par défaut)
video = cv2.VideoCapture(0)
if not video.isOpened():
    print("Erreur: Impossible d'ouvrir la caméra.")
    exit()

# Dictionnaire pour stocker les fichiers audio des mots
audio_files = {}

# Fonction pour générer un fichier audio à partir d'un texte
def generate_audio(text, filename):
    """
    Génère un fichier audio à partir d'un texte.
    :param text: Texte à convertir en audio
    :param filename: Nom du fichier audio
    """
    try:
        tts = gTTS(text=text, lang='en')  # Créer un fichier audio
        tts.save(filename)  # Sauvegarder le fichier audio
        print(f"Fichier audio créé : {filename}")
    except Exception as e:
        print(f"Erreur lors de la création du fichier audio : {e}")

# Générer les fichiers audio pour chaque mot
words = ["Hello", "I Love You", "Yes", "No", "Thank You", "Please", "Stop", "Free Palestine"]
for word in words:
    audio_file = f"{word.lower().replace(' ', '_')}.mp3"
    generate_audio(word, audio_file)
    audio_files[word] = audio_file

# Fonction pour jouer un fichier audio en arrière-plan
def play_audio(filename):
    """
    Joue un fichier audio en arrière-plan.
    :param filename: Chemin du fichier audio
    """
    try:
        pygame.mixer.music.load(filename)  # Charger le fichier audio
        pygame.mixer.music.play()  # Jouer le fichier audio
    except Exception as e:
        print(f"Erreur lors de la lecture audio : {e}")

# Fonction pour reconnaître un geste de la main
def recognize_gesture(landmarks):
    """
    Reconnaît un geste de la main en fonction des positions des points.
    :param landmarks: Liste des points de la main détectés
    :return: Le mot reconnu (par exemple, "Hello", "I Love You", "Yes", "No", etc.)
    """
    # Récupérer les positions des points clés
    thumb_tip = landmarks[4]  # Bout du pouce
    index_tip = landmarks[8]  # Bout de l'index
    middle_tip = landmarks[12]  # Bout du majeur
    ring_tip = landmarks[16]  # Bout de l'annulaire
    pinky_tip = landmarks[20]  # Bout de l'auriculaire

    # Calculer les distances entre les points
    def distance(p1, p2):
        return ((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2) ** 0.5

    # Exemple : Reconnaître "Hello" (main ouverte avec les doigts légèrement écartés)
    if (distance(thumb_tip, index_tip) > 30 and
        distance(index_tip, middle_tip) > 30 and
        distance(middle_tip, ring_tip) > 30 and
        distance(ring_tip, pinky_tip) > 30):
        return "Hello"

    # Exemple : Reconnaître "I Love You" (pouce, index et auriculaire levés)
    if (distance(thumb_tip, index_tip) > 50 and
        distance(index_tip, middle_tip) < 30 and
        distance(middle_tip, ring_tip) < 30 and
        distance(ring_tip, pinky_tip) > 50):
        return "I Love You"

    # Exemple : Reconnaître "Yes" (pouce levé)
    if (distance(thumb_tip, index_tip) > 50 and
        distance(thumb_tip, middle_tip) > 50 and
        distance(thumb_tip, ring_tip) > 50 and
        distance(thumb_tip, pinky_tip) > 50 and
        distance(index_tip, middle_tip) < 30 and
        distance(middle_tip, ring_tip) < 30 and
        distance(ring_tip, pinky_tip) < 30):
        return "Yes"

    # Exemple : Reconnaître "No" (main fermée en poing)
    if (distance(thumb_tip, index_tip) < 30 and
        distance(index_tip, middle_tip) < 30 and
        distance(middle_tip, ring_tip) < 30 and 
        distance(ring_tip, pinky_tip) < 30):
        return "No"

    # Exemple : Reconnaître "Thank You" (main ouverte avec un mouvement de va-et-vient)
    if distance(thumb_tip, middle_tip) < 20:
        return "I Love You"
    if distance(thumb_tip,  pinky_tip) < 20:
        return "Free Palestine"
    if distance(thumb_tip,  ring_tip) < 20:
        return "stop"

    # Exemple : Reconnaître "Please" (main ouverte avec les doigts légèrement repliés)
    if (distance(thumb_tip, index_tip) > 30 and
        distance(index_tip, middle_tip) > 30 and
        distance(middle_tip, ring_tip) > 30 and
        distance(ring_tip, pinky_tip) > 30 and
        distance(thumb_tip, pinky_tip) < 50):
        return "Please"

    # Exemple : Reconnaître "Stop" (main ouverte avec la paume face à la caméra)
    if (distance(thumb_tip, index_tip) > 50 and
        distance(index_tip, middle_tip) > 50 and
        distance(middle_tip, ring_tip) > 50 and
        distance(ring_tip, pinky_tip) > 50 and
        distance(thumb_tip, pinky_tip) > 50):
        return "Stop"

    # Exemple : Reconnaître "Free Palestine" (geste symbolique)
    if (distance(thumb_tip, index_tip) > 50 and
        distance(index_tip, middle_tip) > 50 and
        distance(middle_tip, ring_tip) > 50 and
        distance(ring_tip, pinky_tip) > 50 and
        distance(thumb_tip, pinky_tip) > 50 and
        distance(thumb_tip, middle_tip) < 30):
        return "Free Palestine"

    # Si aucun geste n'est reconnu
    return ""

# Boucle principale de traitement vidéo
last_gesture = ""  # Pour éviter de répéter le même mot plusieurs fois
while True:
    # Lire une frame de la caméra
    ret, image = video.read()
    if not ret:
        print("Erreur: Impossible de lire la frame de la caméra.")
        continue  # Passer à l'itération suivante au lieu de quitter

    # Détecter les mains (avec dessin des points)
    results = Hand_Detector.findHands(image, draw=True)  # draw=True pour afficher les points

    # Si une main est détectée
    if results and len(results[0]) > 0:
        landmarks = results[0][0]['lmList']  # Liste des points de la main

        # Reconnaître le geste de la main
        gesture = recognize_gesture(landmarks)

        # Afficher le mot reconnu à l'écran
        if gesture != last_gesture:
            cvzone.putTextRect(image, f"Gesture: {gesture}", (30, 50), colorR=(0, 255, 0))
            last_gesture = gesture

            # Jouer le fichier audio correspondant au mot détecté
            if gesture in audio_files:
                play_audio(audio_files[gesture])

    # Afficher l'image
    cv2.imshow('Camera', image)

    # Appuyez sur 'q' pour quitter
    if cv2.waitKey(20) == ord('q'):
        break

# Libérer la caméra et fermer les fenêtres
video.release()
cv2.destroyAllWindows()

# Supprimer les fichiers audio temporaires après utilisation
for word, audio_file in audio_files.items():
    if os.path.exists(audio_file):
        os.remove(audio_file)
        print(f"Fichier audio supprimé : {audio_file}")