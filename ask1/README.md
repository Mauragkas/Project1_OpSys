# Αναφορά script διαχείρισης αρχείων buisness

## Overview
Το παρεχόμενο Bash shell script έχει σχεδιαστεί για τη διαχείριση ενός αρχείου CSV που περιέχει πληροφορίες επιχείρησεων. Προσφέρει ενα TUI προς το χρήστη για διάφορες λειτουργίες, όπως προβολή, τροποποίηση και αποθήκευση επιχειρηματικών στοιχείων.

## Λειτουργίες και εργαλεία που χρησιμοποιηθήκαν

### 1. Select Business File (select_business_file)
- **Περιγραφή**: Επιτρέπει στον χρήστη να καθορίσει μια custom διαδρομή αρχείου για το αρχείο επιχειρηματικών δεδομένων ή να χρησιμοποιήσει ένα προεπιλεγμένο αρχείο με το όνομα `Businesses.csv`.
- **Εργαλεία που χρησιμοποιούνται**:
     - `read`: Για να καταγραφούν τα δεδομένα χρήστη.
     - Δηλώσεις `if`: Για ελέγχους υπό όρους (π.χ. εάν η διαδρομή αρχείου είναι κενή ή το αρχείο υπάρχει).
     - `touch`: Για να δημιουργηθεί ένα νέο αρχείο εάν δεν υπάρχει.

### 2. View Business Details (view_business_details)
- **Περιγραφή**: Επιτρέπει στον χρήστη να προβάλλει τα στοιχεία μιας επιχείρησης εισάγοντας τον μοναδικό κωδικό της.
- **Εργαλεία που χρησιμοποιούνται**:
     - `read` και `grep`: Για να ληφθεί ο επιχειρηματικός κωδικός από τον χρήστη και να αναζητηθεί η αντίστοιχη γραμμή στο αρχείο.

### 3. Change Business Element (change_business_element)
- **Περιγραφή**: Προσφέρει functionality για την επεξεργασία συγκεκριμένων πεδίων μιας εγγραφής.
- **Εργαλεία που χρησιμοποιούνται**:
     - `read`, `IFS`, `<<<`, και `grep`: Για την ανάγνωση των δεδομένων χρήστη και την εξαγωγή της σχετικής εγγραφής.
     - Πίνακες: Για να κρατάνε και να χειρίζονται τα πεδία μιας εγγραφής.
     - `sed`: Για την τροποποίηση της επιλεγμένης γραμμής στο αρχείο.
     - `mktemp`: Για να δημιουργηθεί ένα προσωρινό αρχείο κατά τη διαδικασία επεξεργασίας.

### 4. View File (view_file)
- **Περιγραφή**: Εμφανίζει το περιεχόμενο του τρέχοντος αρχείου επιχείρησης.
- **Εργαλεία που χρησιμοποιούνται**:
     - `less`: Ένα βοηθητικό πρόγραμμα γραμμής εντολών για την εμφάνιση των περιεχομένων του αρχείου.

### 5. Save File (save_file)
- **Περιγραφή**: Επιτρέπει στον χρήστη να αποθηκεύσει το τρέχον αρχείο επιχείρησης σε μια καθορισμένη τοποθεσία.
- **Εργαλεία που χρησιμοποιούνται**:
     - `read` και `cp`: Για να ληφθεί η επιθυμητή διαδρομή αποθήκευσης από τον χρήστη και να αντιγραφθεί το αρχείο σε αυτήν τη θέση.

### 6. Main Menu Loop
- **Περιγραφή**: Παρουσιάζει συνεχώς στον χρήστη επιλογές για να εκτελέσει τις παραπάνω λειτουργίες μέχρι να επιλέξει να βγει.
- **Εργαλεία που χρησιμοποιούνται**:
     - Loop `while` και `case`: Για ένα μόνιμο μενού που ανταποκρίνεται στις επιλογές του χρήστη.

### 7. EXIT (exit)
- **Περιγραφή**: Τερματίζει το πρόγραμμα.
- **Εργαλεία που χρησιμοποιούνται**:
     - Τερματισμος script κατόπιν επιλογής του χρήστη.

## Συμπέρασμα
Αυτό το script χρησιμοποιεί αποτελεσματικά τυπικές εντολές Bash για τη διαχείριση μιας απλή business βάση δεδομένων σε CSV. Ωστόσο, για πιο προηγμένες λειτουργίες, ενδέχεται να απαιτούνται πρόσθετοι έλεγχοι διαχείρισης σφαλμάτων και συμβατότητας.