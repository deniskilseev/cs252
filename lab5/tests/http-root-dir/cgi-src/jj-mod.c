/*
 * Submarine ordering form through FAX gateway
 * 
 * Rob McCool
 * 
 */

#include <stdio.h>
#include <string.h>
#ifndef NO_STDLIB_H
#include <stdlib.h>
#else
char *getenv();
#endif

#if 1
#define JJ_FAX "JIMMY_JOHNS_5555680@fax.uiuc.edu"
#else
#define JJ_FAX "wow@foo.bar.com"
#endif

#define PASSWORD "HTTPdRocKs"
#define LF 10
#define CR 13

void getword(char *word, char *line, char stop);
char x2c(char *what);
void unescape_url(char *url);
void plustospace(char *str);

FILE* fd = NULL;

char *sublist[] = {
    "The Pepe Gourmet Sub",
    "Big John Gourmet Sub",
    "Sorry Charlie Gourmet Sub",
    "Turkey Tom Gourmet Sub",
    "Vito Gourmet Sub",
    "Vegetarian Gourmet Sub",
    "Gourmet Smoked Ham Club",
    "Billy Club",
    "Italian Night Club",
    "Hunter's Club",
    "Country Club",
    "The Beach Club"};

char *slimlist[] = {
    "Ham and Cheese",
    "Rare Roast Beef",
    "California Tuna",
    "Sliced Turkey",
    "Salami and Capacola",
    "Double Provolone"};

char *sidelist[] = {
    "Lay's Potato Chips",
    "Jumbo Kosher Dill"};

char *poplist[] = {
    "Pepsi",
    "Mountain Dew",
    "Diet Pepsi",
    "Iced Tea"};

void dump_form()
{
    fprintf(fd, "<TITLE>Form for Submarine Order</TITLE>%c", LF);
    fprintf(fd, "<H1>Jimmy John's Submarine Order Form</H1>%c", LF);
    fprintf(fd, "This form will send a faxed order to Jimmy John's in Champaign. Proper password is requred%c", LF);
    fprintf(fd, "for order to be submitted, otherwise a copy of the order that would have been submitted will%c", LF);
    fprintf(fd, "will be displayed.<P>%c", LF);
    fprintf(fd, "<HR>%c", LF);
    fprintf(fd, "<FORM ACTION=\"jj\">%c", LF);
    fprintf(fd, "Password: <INPUT TYPE=\"text\" NAME=\"pwd\" MAXLENGTH=\"20\"><P>%c", LF);
    fprintf(fd, "<H3>Sub Type</H3>%c", LF);
    fprintf(fd, "Select which you would like of the following:<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"0\">%s:%c", sublist[0], LF);
    fprintf(fd, "Smoked virginia ham and provolone cheese topped with lettuce, tomato, and mayo.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"1\">%s:%c", sublist[1], LF);
    fprintf(fd, "Medium rare shaved roast beef topped with mayo, lettuce, and tomato.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"2\">%s:%c", sublist[2], LF);
    fprintf(fd, "Tuna, mixed with celery, onions, and sauce, topped with lettuce,%c", LF);
    fprintf(fd, "tomato, and alfalfa sprouts.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"3\">%s:%c", sublist[3], LF);
    fprintf(fd, "Turkey breast topped with lettuce, mayo, alfalfa sprouts, and mayo.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"4\">%s:%c", sublist[4], LF);
    fprintf(fd, "Genoa salami and provolone cheese topped with capacola, onion, lettuce, tomato, and Italian sauce.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"5\">%s:%c", sublist[5], LF);
    fprintf(fd, "Layers of provolone cheese, separated by avocado, sprouts, lettuce, tomato, and mayo.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"6\">%s:%c", sublist[6], LF);
    fprintf(fd, "1/4 pound of smoked ham, provolone cheese, topped with lettuce,%c", LF);
    fprintf(fd, "tomato, and mayo.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"7\">%s:%c", sublist[7], LF);
    fprintf(fd, "Shaved roast beef, provolone cheese, french dijon mustard, topped with shaved ham, lettuce,%c", LF);
    fprintf(fd, "tomato, and mayo.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"8\">%s:%c", sublist[8], LF);
    fprintf(fd, "Genoa salami, Italian capacola, smoked ham, and provolone cheese topped with lettuce,%c", LF);
    fprintf(fd, "tomato, onions, mayo, and Italian sauce.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"9\">%s:%c", sublist[9], LF);
    fprintf(fd, "1/4 pound of sliced roast beef, provolone cheese, topped with lettuce, tomato, and mayo.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"10\">%s:%c", sublist[10], LF);
    fprintf(fd, "Turkey breast, smoked ham, and provolonecheese topped with lettuce, tomato, and mayo.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sub\" VALUE=\"11\">%s:%c", sublist[11], LF);
    fprintf(fd, "Turkey breast, avocado, and cheese topped with lettuce, mayo, alfalfa, and tomato.<P>%c", LF);
    fprintf(fd, "<H3>Slim Jim Subs</H3>%c", LF);
    fprintf(fd, "Subs without veggies or sauce.<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"slj\" VALUE=\"0\">%s<P>%c", slimlist[0], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"slj\" VALUE=\"1\">%s<P>%c", slimlist[1], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"slj\" VALUE=\"2\">%s<P>%c", slimlist[2], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"slj\" VALUE=\"3\">%s<P>%c", slimlist[3], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"slj\" VALUE=\"4\">%s<P>%c", slimlist[4], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"slj\" VALUE=\"5\">%s<P>%c", slimlist[5], LF);
    fprintf(fd, "<H3>Side orders</H3>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sde\" VALUE=\"0\">%s<P>%c", sidelist[0], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"sde\" VALUE=\"1\">%s<P>%c", sidelist[1], LF);
    fprintf(fd, "<H3>Drinks</H3>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"pop\" VALUE=\"0\">%s<P>%c", poplist[0], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"pop\" VALUE=\"1\">%s<P>%c", poplist[1], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"pop\" VALUE=\"2\">%s<P>%c", poplist[2], LF);
    fprintf(fd, "<INPUT TYPE=\"checkbox\" NAME=\"pop\" VALUE=\"3\">%s<P>%c", poplist[3], LF);
    fprintf(fd, "<H3>Your Address, Phone Number, and Name</H3>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"text\" NAME=\"name\" MAXLENGTH=\"32\">Name<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"text\" NAME=\"adr\" MAXLENGTH=\"64\">Address<P>%c", LF);
    fprintf(fd, "<INPUT TYPE=\"text\" NAME=\"phone\" MAXLENGTH=\"10\">Phone Number<P>%c", LF);
    fprintf(fd, "<INPUT type=\"submit\">%c", LF);
    fprintf(fd, "</FORM>%c", LF);
}

void print_error(char *reason)
{
    fprintf(fd, "<TITLE>Order Not Submitted</TITLE>%c", LF);
    fprintf(fd, "<H1>Order Not Submitted</H1>%c", LF);
    fprintf(fd, "Your order has not been submitted, because %s.<P>%c", reason, LF);
}

void httprun(int ssock, char *query_string)
{
    fd = fdopen(ssock, "w");
    if (fd == NULL) {
        perror("fdopen");
    }
    register int x, m = 0;
    char *cl;
    char w[256];
    char tfile[L_tmpnam];
    int subs, slims, sides, drinks, allow;
    char name[32];
    char phone[10];
    char address[64];
    FILE *tfp, *order;

    fprintf(fd, "Content-type: text/html%c%c%c%c", CR, LF, CR, LF);

    cl = query_string;
    if ((!cl) || (!cl[0]))
    {
        dump_form();
        return;
    }

    mkstemp(tfile);
    if (!(tfp = fopen(tfile, "w")))
    {
        fprintf(fd, "<TITLE>Server Error</TITLE>%c", LF);
        fprintf(fd, "<H1>Server Error</H1>%c", LF);
        fprintf(fd, "Server unable to get a temporary file. Please try again later.<P>%c", LF);
        return;
    }

    subs = 0;
    slims = 0;
    sides = 0;
    drinks = 0;
    allow = 0;
    name[0] = '\0';
    phone[0] = '\0';
    address[0] = '\0';

    for (x = 0; cl[0] != '\0'; x++)
    {
        m = x;
        getword(w, cl, '=');
        plustospace(w);
        unescape_url(w);
        if (!strcmp(w, "pwd"))
        {
            getword(w, cl, '&');
            plustospace(w);
            unescape_url(w);
            allow = (strcmp(w, PASSWORD) ? 0 : 1);
        }
        if (!strcmp(w, "sub"))
        {
            getword(w, cl, '&');
            plustospace(w);
            unescape_url(w);
            subs |= (1 << atoi(w));
        }
        else if (!strcmp(w, "slj"))
        {
            getword(w, cl, '&');
            plustospace(w);
            unescape_url(w);
            slims |= (1 << atoi(w));
        }
        else if (!strcmp(w, "sde"))
        {
            getword(w, cl, '&');
            plustospace(w);
            unescape_url(w);
            sides |= (1 << atoi(w));
        }
        else if (!strcmp(w, "pop"))
        {
            getword(w, cl, '&');
            plustospace(w);
            unescape_url(w);
            drinks |= (1 << atoi(w));
        }
        else if (!strcmp(w, "name"))
        {
            getword(w, cl, '&');
            plustospace(w);
            unescape_url(w);
            strcpy(name, w);
        }
        else if (!strcmp(w, "phone"))
        {
            getword(w, cl, '&');
            plustospace(w);
            unescape_url(w);
            strcpy(phone, w);
        }
        else if (!strcmp(w, "adr"))
        {
            getword(w, cl, '&');
            plustospace(w);
            unescape_url(w);
            strcpy(address, w);
        }
    }

    if (!name[0]) {
        print_error("you didn't give your name");
        return;
    }
    if (!address[0]) {
        print_error("you didn't give your address");
        return;
    }
    if (!phone[0]) {
        print_error("you didn't give your phone number");
        return;
    }
    if ((!subs) && (!slims) && (!sides) && (!drinks)) {
        print_error("you didn't order anything");
        return;
    }

    if (allow)
    {
        char t[256];
        sprintf(t, "/bin/mail %s", JJ_FAX);
        if (!(order = popen(t, "w"))) {
            print_error("the server was unable to open a pipe to mail");
            return;
        }
        fprintf(fd, "<TITLE>Order Sent</TITLE>%c", LF);
        fprintf(fd, "<H1>Order Sent</H1>%c", LF);
        fprintf(fd, "Your order has been sent to the UIUC e-mail to FAX gateway.<P>%c", LF);
    }
    else
    {
        fprintf(fd, "<TITLE>Your Order</TITLE>%c", LF);
        fprintf(fd, "<H1>Your Order</H1>%c", LF);
        fprintf(fd, "This is how your order would have looked if it had been sent.<P><PLAINTEXT>%c", LF);
        order = stdout;
    }

    fprintf(order, "My name is %s, and I would like to have the following%c",
            name, LF);
    fprintf(order, "order delivered to %s:%c%c", address, LF, LF);
    for (x = 0; x < 12; x++)
        if (subs & (1 << x))
            fprintf(order, "\t(1) %s%c", sublist[x], LF);
    for (x = 0; x < 6; x++)
        if (slims & (1 << x))
            fprintf(order, "\t(1) %s Slim Jim%c", slimlist[x], LF);
    for (x = 0; x < 2; x++)
        if (sides & (1 << x))
            fprintf(order, "\t(1) %s%c", sidelist[x], LF);
    for (x = 0; x < 4; x++)
        if (drinks & (1 << x))
            fprintf(order, "\t(1) %s%c", poplist[x], LF);
    fprintf(order, "%cPlease feel free to call me at %s if there is any%c", LF,
            phone, LF);
    fprintf(order, "problem. Thank you.%c%c.%c", LF, LF, LF);
    fclose(order);
    fclose(fd);
    return;
}
