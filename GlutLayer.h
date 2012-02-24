#ifndef GlutLayer_H
#define GlutLayer_H

class UI;

void registerUI(UI &);
void unregisterUI();

void display(void);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void special(int key, int x, int y);
void specialUp(int key, int x, int y);
void reshape(int width, int height);
void idle(void);

#endif
