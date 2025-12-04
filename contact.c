#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "populate.h"

//validation stages

int Validate_Name(char name[]) 
{
    for (int i = 0; name[i] != '\0'; i++) 
    {
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
              (name[i] >= 'A' && name[i] <= 'Z') ||
              name[i] == ' '))
            return 0;
    }
    return 1;
}

int Validate_Email(char gm[]) 
{
    int at = 0, dot = 0;
    for (int i = 0; gm[i] != '\0'; i++) 
    {
        if (gm[i] == '@') 
        at = 1;
        if (gm[i] == '.' && at) 
        dot = 1;
    }
    return (at && dot);
}

int digit(char ph[]) 
{
    for (int i = 0; ph[i] != '\0'; i++) 
    {
        if (!(ph[i] >= '0' && ph[i] <= '9'))
            return 0;
    }
    return 1;
}
int Validate_Ph(char ph[]) 
{
    return (strlen(ph) == 10 && digit(ph));
}
//duplicate phone and email 
int Duplicate_Ph(AddressBook *addr, char ph[]) 
{
    for (int i = 0; i < addr->contactCount; i++) 
    {
        if (strcmp(addr->contacts[i].phone, ph) == 0)
            return 1;
    }
    return 0;
}

int Duplicate_Email(AddressBook *addr, char gm[]) 
{
    for (int i = 0; i < addr->contactCount; i++) 
    {
        if (strcmp(addr->contacts[i].email, gm) == 0)
            return 1;
    }
    return 0;
}



void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}

//List of contacts
void listContacts(AddressBook *addressBook) 
{
    printf("\n------------------ Contact List -------------------\n");
    if (addressBook->contactCount == 0) 
    {
        printf("No contacts found.\n");
        return;
    }
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        printf("%d. Name: %s | Phone: %s | Email: %s\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
}
// Search contact
void searchContact(AddressBook *addressBook) 
{
    int choice, found = 0;
    char key[50];
    printf("\nSearch by:\n1. Name\n2. Phone\n3. Email\nEnter choice: ");
    scanf("%d", &choice);

    printf("Enter search value: ");
    scanf(" %[^\n]", key);

    for (int i = 0; i < addressBook->contactCount; i++) {
        if ((choice == 1 && strcmp(addressBook->contacts[i].name, key) == 0) ||
            (choice == 2 && strcmp(addressBook->contacts[i].phone, key) == 0) ||
            (choice == 3 && strcmp(addressBook->contacts[i].email, key) == 0)) {
            printf("%d. %s | %s | %s\n",
                   i + 1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
            found = 1;
        }
    }

    if (!found)
        printf("No contact found.\n");
}

//Create contact
void createContact(AddressBook *addressBook) 
{
    if (addressBook->contactCount >= MAX_CONTACTS) {
        printf("Address book full!\n");
        return;
    }

    Contact newContact;

    while (1) 
    {
        printf("Enter name: ");
        scanf(" %[^\n]", newContact.name);
        if (!Validate_Name(newContact.name))
            printf("Invalid name! Only alphabets are allowed.\n");
        else
            break;
    }

    while (1) 
    {
        printf("Enter phone number: ");
        scanf(" %[^\n]", newContact.phone);
        if (!Validate_Ph(newContact.phone))
            printf("Invalid phone!\n");
        else if (Duplicate_Ph(addressBook, newContact.phone))
            printf("Phone already exists!\n");
        else
            break;
    }

    while (1) 
    {
        printf("Enter email: ");
        scanf(" %[^\n]", newContact.email);
        if (!Validate_Email(newContact.email))
            printf("Invalid email!\n");
        else if (Duplicate_Email(addressBook, newContact.email))
            printf("Email already exists!\n");
        else
            break;
    }

    addressBook->contacts[addressBook->contactCount++] = newContact;
    printf("Contact added successfully!\n");
}


// Edit contact
void editContact(AddressBook *addressBook) 
{
    int choice;
    char key[50];
    printf("\nSearch Contact to Edit by:\n1. Name\n2. Phone\n3. Email\nEnter choice: ");
    scanf("%d", &choice);

    printf("Enter search value: ");
    scanf(" %[^\n]", key);

    int foundCount = 0;

    printf("\nMatching contacts:\n");
    for (int i = 0; i < addressBook->contactCount; i++) {
        if ((choice == 1 && strcmp(addressBook->contacts[i].name, key) == 0) ||
            (choice == 2 && strcmp(addressBook->contacts[i].phone, key) == 0) ||
            (choice == 3 && strcmp(addressBook->contacts[i].email, key) == 0)) {
            printf("%d. Name: %s | Phone: %s | Email: %s\n",
                   i + 1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
            foundCount++;
        }
    }

    if (foundCount == 0) {
        printf("No contact found.\n");
        return;
    }

    int index;
    if (foundCount > 1)
        printf("Enter the serial number of the contact to edit: ");
    else
        printf("Enter the serial number to edit: ");

    scanf("%d", &index);

    if (index < 1 || index > addressBook->contactCount) {
        printf("Invalid index.\n");
        return;
    }

    index--; // convert to 0-based index

    printf("\nEdit Menu:\n1. Edit Name\n2. Edit Phone\n3. Edit Email\nEnter choice: ");
    int ch;
    scanf("%d", &ch);

    switch (ch) {
        case 1:
            while (1) {
                printf("Enter new name: ");
                scanf(" %[^\n]", addressBook->contacts[index].name);
                if (!Validate_Name(addressBook->contacts[index].name))
                    printf("Invalid name! Retry.\n");
                else break;
            }
            break;
        case 2:
            while (1) {
                printf("Enter new phone: ");
                scanf(" %[^\n]", addressBook->contacts[index].phone);
                if (!Validate_Ph(addressBook->contacts[index].phone))
                    printf("Invalid phone number! Retry.\n");
                else break;
            }
            break;
        case 3:
            while (1) {
                printf("Enter new email: ");
                scanf(" %[^\n]", addressBook->contacts[index].email);
                if (!Validate_Email(addressBook->contacts[index].email))
                    printf("Invalid email! Retry.\n");
                else break;
            }
            break;
        default:
            printf("Invalid option.\n");
    }

    printf("Contact updated successfully!\n");
}


// Delete contact
void deleteContact(AddressBook *addressBook) 
{
    int choice;
    char key[50];
    printf("\nSearch Contact to Delete by:\n1. Name\n2. Phone\n3. Email\nEnter your choice: ");
    scanf("%d", &choice);

    printf("Enter value: ");
    scanf(" %[^\n]", key);

    int foundCount = 0;

    printf("\nMatching contacts:\n");
    for (int i = 0; i < addressBook->contactCount; i++) {
        if ((choice == 1 && strcmp(addressBook->contacts[i].name, key) == 0) ||
            (choice == 2 && strcmp(addressBook->contacts[i].phone, key) == 0) ||
            (choice == 3 && strcmp(addressBook->contacts[i].email, key) == 0)) 
            {
            printf("%d. Name: %s | Phone: %s | Email: %s\n",
                   i + 1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);
            foundCount++;
        }
    }

    if (foundCount == 0) {
        printf("No contact found.\n");
        return;
    }

    int index;
    if (foundCount > 1)
        printf("Enter the serial number of the contact to delete: ");
    else
        printf("Enter the serial number to delete: ");

    scanf("%d", &index);

    if (index < 1 || index > addressBook->contactCount) {
        printf("Invalid index.\n");
        return;
    }

    index--; // convert to 0-based index

    // Shift contacts after deleted one
    for (int i = index; i < addressBook->contactCount - 1; i++) {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--;

    printf("Contact deleted successfully.\n");
}


void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contacts.csv", "w");
    if (fp == NULL) 
    {
        printf("Error\n");
        return;
    }

    fprintf(fp, "%d\n", addressBook->contactCount);
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        fprintf(fp, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }
    fclose(fp);
    printf("Contacts saved successfully\n");
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contacts.csv", "r");
    if (fp == NULL) 
    {
        printf("No contacts found\n");
        populateAddressBook(addressBook);
        return;
    }

    int count;
    if (fscanf(fp, "%d\n", &count) != 1)
    {
        fclose(fp);
        populateAddressBook(addressBook);
        return;
    }

    addressBook->contactCount = 0;
    for (int i = 0; i < count && i < MAX_CONTACTS; i++) {
        fscanf(fp, "%49[^,],%19[^,],%49[^\n]\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
        addressBook->contactCount++;
    }

    fclose(fp);
    printf("Loaded %d contacts from contacts.csv\n", addressBook->contactCount);
}


