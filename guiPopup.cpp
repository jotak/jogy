#include "guiPopup.h"
#include "../Data/LocalisationTool.h"
#include "FrameEffects/EffectComeIn.h"
#include "FrameEffects/guiFrameOutro.h"
#include "FrameEffects/guiFrameMouseFocus.h"
#include "../Display/DisplayEngine.h"
#include "../Display/TextureEngine.h"

// -----------------------------------------------------------------
// Name : guiPopup
//  Constructor ; do not use (private)
// -----------------------------------------------------------------
guiPopup::guiPopup() : guiFrame()
{
    m_fTimer = -1;
    m_bAutoClose = false;
}

// -----------------------------------------------------------------
// Name : ~guiPopup
//  Destructor
// -----------------------------------------------------------------
guiPopup::~guiPopup()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiPopup::update(double delta)
{
    if (!m_bVisible) {
        return;
    }
    guiFrame::update(delta);
    if (m_fTimer >= 1)
    {
        m_fTimer -= delta;
        if (m_fTimer < 1)
        {
            guiFrameOutro * pEffect = new guiFrameOutro(POPUP_EFFECT_OUTRO, 1.0f, EFFECT_DELFRM_ON_FINISHED);
            addEffect(pEffect);
            pEffect->setActive(true);
        }
    }
    if (m_bAutoClose)
    {
        guiComponent * pCpnt = getClickedComponent();
        if (pCpnt != NULL)
        {
            if (pCpnt->getId() == "OkButton")
            {
                getDocument()->close();
                m_bAutoClose = false;
            }
        }
    }
}

// -----------------------------------------------------------------
// Name : getButton
// -----------------------------------------------------------------
guiButton * guiPopup::getButton(int iButton)
{
    char str[64] = "0";
    str[0] = iButton - '0';
    return (guiButton*) getDocument()->getComponent(str);
}

// -----------------------------------------------------------------
// Name : createEmptyPopup
//  Static default constructor
// -----------------------------------------------------------------
guiPopup * guiPopup::createEmptyPopup()
{
    // Create empty popup (frame)
    guiPopup * pPopup = new guiPopup();
    Texture * frmtex[8];
    frmtex[0] = _tex->findTexture("gui/interface:FrmTL");
    frmtex[1] = _tex->findTexture("gui/interface:FrmTC");
    frmtex[2] = _tex->findTexture("gui/interface:FrmTR");
    frmtex[3] = _tex->findTexture("gui/interface:FrmCL");
    frmtex[4] = _tex->findTexture("gui/interface:FrmCR");
    frmtex[5] = _tex->findTexture("gui/interface:FrmBL");
    frmtex[6] = _tex->findTexture("gui/interface:FrmBC");
    frmtex[7] = _tex->findTexture("gui/interface:FrmBR");
    pPopup->init(
        FP_Floating,
        FB_FitFrameToDocument,
        FB_FitFrameToDocument,
        0, 0, 0, 0, frmtex, "EmptyPopup", 0, 0, 1, 1);

    // Create popup inner document
    guiDocument * pDoc = new guiPopupDocument();
    pDoc->init(
        "EmptyPopupDocument",
        _tex->findTexture("gui/interface:WinBg"),
        0, 0, 1, 1);
    pPopup->setDocument(pDoc);

    guiFrameEffect * pEffect = new guiFrameMouseFocus(POPUP_EFFECT_FOCUS, 1.0f);
    pPopup->addEffect(pEffect);

    pEffect = new EffectComeIn(POPUP_EFFECT_INTRO, 0.3f);
    pPopup->addEffect(pEffect);
    pEffect->reset();

    return pPopup;
}

// -----------------------------------------------------------------
// Name : createOkAutoclosePopup
// -----------------------------------------------------------------
guiPopup * guiPopup::createOkAutoclosePopup(string sText)
{
    // Create empty popup (frame)
    guiPopup * pPopup = new guiPopup();
    Texture * frmtex[8];
    frmtex[0] = _tex->findTexture("gui/interface:FrmTL");
    frmtex[1] = _tex->findTexture("gui/interface:FrmTC");
    frmtex[2] = _tex->findTexture("gui/interface:FrmTR");
    frmtex[3] = _tex->findTexture("gui/interface:FrmCL");
    frmtex[4] = _tex->findTexture("gui/interface:FrmCR");
    frmtex[5] = _tex->findTexture("gui/interface:FrmBL");
    frmtex[6] = _tex->findTexture("gui/interface:FrmBC");
    frmtex[7] = _tex->findTexture("gui/interface:FrmBR");
    pPopup->init(
        FP_Floating,
        FB_FitFrameToDocument,
        FB_FitFrameToDocument,
        0, 0, 0, 0, frmtex, "OkAutoclosePopup", 0, 0, 1, 1);

    // Init position for components
    int iWidth = 250;
    int yPxl = 10;

    // Create popup inner document
    guiDocument * pDoc = new guiPopupDocument();
    pDoc->init(
        "OkAutoclosePopupDocument",
        _tex->findTexture("gui/interface:WinBg"),
        0, 0, iWidth, 1);

    // Top label
    guiLabel * pLbl = new guiLabel();
    pLbl->init(sText, H2_FONT, H2_COLOR, "TopLabe", 0, 0, iWidth - 10, 0);
    pLbl->moveTo((iWidth - pLbl->getWidth()) / 2, yPxl);
    pDoc->addComponent(pLbl);

    // Button
    yPxl += pLbl->getHeight() + 20;
    guiButton * pBtn = guiButton::createDefaultNormalButton(i18n->getTextUCFirst("OK"), "OkButton");
    pBtn->moveTo(iWidth / 2 - pBtn->getWidth() / 2, yPxl);
    pDoc->addComponent(pBtn);

    // Init popup
    pDoc->setDimensions(iWidth, yPxl + pBtn->getHeight() + 10);
    pPopup->setDocument(pDoc);
    pPopup->m_bAutoClose = true;

    guiFrameEffect * pEffect = new guiFrameMouseFocus(POPUP_EFFECT_FOCUS, 1.0f);
    pPopup->addEffect(pEffect);

    pEffect = new EffectComeIn(POPUP_EFFECT_INTRO, 0.3f);
    pPopup->addEffect(pEffect);
    pEffect->reset();

    return pPopup;
}

// -----------------------------------------------------------------
// Name : createOkCancelPopup
//  Static default constructor
// -----------------------------------------------------------------
guiPopup * guiPopup::createOkCancelPopup(string sText)
{
    // Create empty popup (frame)
    guiPopup * pPopup = new guiPopup();
    Texture * frmtex[8];
    frmtex[0] = _tex->findTexture("gui/interface:FrmTL");
    frmtex[1] = _tex->findTexture("gui/interface:FrmTC");
    frmtex[2] = _tex->findTexture("gui/interface:FrmTR");
    frmtex[3] = _tex->findTexture("gui/interface:FrmCL");
    frmtex[4] = _tex->findTexture("gui/interface:FrmCR");
    frmtex[5] = _tex->findTexture("gui/interface:FrmBL");
    frmtex[6] = _tex->findTexture("gui/interface:FrmBC");
    frmtex[7] = _tex->findTexture("gui/interface:FrmBR");
    pPopup->init(
        FP_Floating,
        FB_FitFrameToDocument,
        FB_FitFrameToDocument,
        0, 0, 0, 0, frmtex, "OkCancelPopup", 0, 0, 1, 1);

    // Init position for components
    int iWidth = 250;
    int yPxl = 10;

    // Create popup inner document
    guiDocument * pDoc = new guiPopupDocument();
    pDoc->init(
        "OkCancelPopupDocument",
        _tex->findTexture("gui/interface:WinBg"),
        0, 0, iWidth, 1);

    // Top label
    guiLabel * pLbl = new guiLabel();
    pLbl->init(sText, H2_FONT, H2_COLOR, "TopLabe", 0, 0, iWidth - 10, 0);
    pLbl->moveTo((iWidth - pLbl->getWidth()) / 2, yPxl);
    pDoc->addComponent(pLbl);

    // Buttons
    yPxl += pLbl->getHeight() + 20;
    guiButton * pBtn = guiButton::createDefaultNormalButton(i18n->getTextUCFirst("OK"), "OkButton");
    pBtn->moveTo(3 * iWidth / 4 - pBtn->getWidth() / 2, yPxl);
    pDoc->addComponent(pBtn);

    pBtn = guiButton::createDefaultNormalButton(i18n->getTextUCFirst("CANCEL"), "CancelButton");
    pBtn->moveTo(iWidth / 4 - pBtn->getWidth() / 2, yPxl);
    pDoc->addComponent(pBtn);

    // Init popup
    pDoc->setDimensions(iWidth, yPxl + pBtn->getHeight() + 10);
    pPopup->setDocument(pDoc);

    guiFrameEffect * pEffect = new guiFrameMouseFocus(POPUP_EFFECT_FOCUS, 1.0f);
    pPopup->addEffect(pEffect);

    pEffect = new EffectComeIn(POPUP_EFFECT_INTRO, 0.3f);
    pPopup->addEffect(pEffect);
    pEffect->reset();

    return pPopup;
}

// -----------------------------------------------------------------
// Name : createYesNoPopup
//  Static default constructor
// -----------------------------------------------------------------
guiPopup * guiPopup::createYesNoPopup(string sText)
{
    // Create empty popup (frame)
    guiPopup * pPopup = new guiPopup();
    Texture * frmtex[8];
    frmtex[0] = _tex->findTexture("gui/interface:FrmTL");
    frmtex[1] = _tex->findTexture("gui/interface:FrmTC");
    frmtex[2] = _tex->findTexture("gui/interface:FrmTR");
    frmtex[3] = _tex->findTexture("gui/interface:FrmCL");
    frmtex[4] = _tex->findTexture("gui/interface:FrmCR");
    frmtex[5] = _tex->findTexture("gui/interface:FrmBL");
    frmtex[6] = _tex->findTexture("gui/interface:FrmBC");
    frmtex[7] = _tex->findTexture("gui/interface:FrmBR");
    pPopup->init(
        FP_Floating,
        FB_FitFrameToDocument,
        FB_FitFrameToDocument,
        0, 0, 0, 0, frmtex, "YesNoPopup", 0, 0, 1, 1);

    // Init position for components
    int iWidth = 250;
    int yPxl = 10;

    // Create popup inner document
    guiDocument * pDoc = new guiPopupDocument();
    pDoc->init(
        "YesNoPopupDocument",
        _tex->findTexture("gui/interface:WinBg"),
        0, 0, iWidth, 1);

    // Top label
    guiLabel * pLbl = new guiLabel();
    pLbl->init(sText, H2_FONT, H2_COLOR, "TopLabe", 0, 0, iWidth - 10, 0);
    pLbl->moveTo((iWidth - pLbl->getWidth()) / 2, yPxl);
    pDoc->addComponent(pLbl);

    // Buttons
    yPxl += pLbl->getHeight() + 20;
    guiButton * pBtn = guiButton::createDefaultNormalButton(i18n->getTextUCFirst("YES"), "YesButton");
    pBtn->moveTo(3 * iWidth / 4 - pBtn->getWidth() / 2, yPxl);
    pDoc->addComponent(pBtn);

    pBtn = guiButton::createDefaultNormalButton(i18n->getTextUCFirst("NO"), "NoButton");
    pBtn->moveTo(iWidth / 4 - pBtn->getWidth() / 2, yPxl);
    pDoc->addComponent(pBtn);

    // Init popup
    pDoc->setDimensions(iWidth, yPxl + pBtn->getHeight() + 10);
    pPopup->setDocument(pDoc);

    guiFrameEffect * pEffect = new guiFrameMouseFocus(POPUP_EFFECT_FOCUS, 1.0f);
    pPopup->addEffect(pEffect);

    pEffect = new EffectComeIn(POPUP_EFFECT_INTRO, 0.3f);
    pPopup->addEffect(pEffect);
    pEffect->reset();

    return pPopup;
}

// -----------------------------------------------------------------
// Name : createTextAndMultiButtonsPopup
//  Static default constructor
// -----------------------------------------------------------------
guiPopup * guiPopup::createTextAndMultiButtonsPopup(string sText, int iNbButtons, int iWidth)
{
    // Create empty popup (frame)
    guiPopup * pPopup = new guiPopup();
    Texture * frmtex[8];
    frmtex[0] = _tex->findTexture("gui/interface:FrmTL");
    frmtex[1] = _tex->findTexture("gui/interface:FrmTC");
    frmtex[2] = _tex->findTexture("gui/interface:FrmTR");
    frmtex[3] = _tex->findTexture("gui/interface:FrmCL");
    frmtex[4] = _tex->findTexture("gui/interface:FrmCR");
    frmtex[5] = _tex->findTexture("gui/interface:FrmBL");
    frmtex[6] = _tex->findTexture("gui/interface:FrmBC");
    frmtex[7] = _tex->findTexture("gui/interface:FrmBR");
    pPopup->init(
        FP_Floating,
        FB_FitFrameToDocument,
        FB_FitFrameToDocument,
        0, 0, 0, 0, frmtex, "MultiButtonsPopup", 0, 0, 1, 1);

    // Create popup inner document
    guiDocument * pDoc = new guiPopupDocument();
    pDoc->init(
        "MultiButtonsPopupDocument",
        _tex->findTexture("gui/interface:WinBg"),
        0, 0, iWidth, 1);

    // Init position for components
    int yPxl = 10;

    // Top label
    guiLabel * pLbl = new guiLabel();
    pLbl->init(sText, H2_FONT, H2_COLOR, "TopLabe", 0, 0, iWidth - 10, 0);
    pLbl->moveTo((iWidth - pLbl->getWidth()) / 2, yPxl);
    pDoc->addComponent(pLbl);

    // Buttons
    yPxl += pLbl->getHeight() + 20;
    int height = yPxl;
    char str[64] = "0";
    int boxw = iNbButtons > 0 ? iWidth / iNbButtons : 0;
    for (int i = 0; i < iNbButtons; i++)
    {
        str[0] = '0' + i;
        guiButton * pBtn = guiButton::createDefaultNormalButton("", str);
        pBtn->moveTo(i * boxw + boxw / 2 - pBtn->getWidth() / 2, yPxl);
        pDoc->addComponent(pBtn);
        height = yPxl + pBtn->getHeight();
    }

    // Init popup
    pDoc->setDimensions(iWidth, height + 10);
    pPopup->setDocument(pDoc);

    guiFrameEffect * pEffect = new guiFrameMouseFocus(POPUP_EFFECT_FOCUS, 1.0f);
    pPopup->addEffect(pEffect);

    pEffect = new EffectComeIn(POPUP_EFFECT_INTRO, 0.3f);
    pPopup->addEffect(pEffect);
    pEffect->reset();

    return pPopup;
}

// -----------------------------------------------------------------
// Name : createTextInputPopup
//  Static default constructor
// -----------------------------------------------------------------
guiPopup * guiPopup::createTextInputPopup(string sText, int iNbLines, bool bMultiLines, int iBoxWidth)
{
    // Create empty popup (frame)
    guiPopup * pPopup = new guiPopup();
    Texture * frmtex[8];
    frmtex[0] = _tex->findTexture("gui/interface:FrmTL");
    frmtex[1] = _tex->findTexture("gui/interface:FrmTC");
    frmtex[2] = _tex->findTexture("gui/interface:FrmTR");
    frmtex[3] = _tex->findTexture("gui/interface:FrmCL");
    frmtex[4] = _tex->findTexture("gui/interface:FrmCR");
    frmtex[5] = _tex->findTexture("gui/interface:FrmBL");
    frmtex[6] = _tex->findTexture("gui/interface:FrmBC");
    frmtex[7] = _tex->findTexture("gui/interface:FrmBR");
    pPopup->init(
        FP_Floating,
        FB_FitFrameToDocument,
        FB_FitFrameToDocument,
        0, 0, 0, 0, frmtex, "TextInputPopup", 0, 0, 1, 1);

    // Create popup inner document
    guiDocument * pDoc = new guiPopupDocument();
    pDoc->init(
        "TextInputPopupDocument",
        _tex->findTexture("gui/interface:WinBg"),
        0, 0, 1, 1);

    // Init size & position for components
    int iWidth = iBoxWidth + 10;
    int yPxl = 10;

    // Top label
    guiLabel * pLbl = new guiLabel();
    pLbl->init(sText, H2_FONT, H2_COLOR, "TopLabe", 0, 0, iBoxWidth, 0);
    pLbl->moveTo((iWidth - pLbl->getWidth()) / 2, yPxl);
    pDoc->addComponent(pLbl);

    yPxl += pLbl->getHeight() + 10;

    // Edit box
    guiEditBox * pBox = guiEditBox::createDefaultEditBox(iNbLines, bMultiLines, iBoxWidth, "DefaultEditBox");
    pBox->moveTo(5, yPxl);
    pDoc->addComponent(pBox);
    pBox->setFocus();
    yPxl += pBox->getHeight() + 20;

    // Buttons
    guiButton * pBtn = guiButton::createDefaultNormalButton(i18n->getTextUCFirst("CANCEL"), "CancelButton");
    pBtn->moveTo(iWidth / 4 - pBtn->getWidth() / 2, yPxl);
    pDoc->addComponent(pBtn);

    pBtn = guiButton::createDefaultNormalButton(i18n->getTextUCFirst("OK"), "OkButton");
    pBtn->moveTo(3 * iWidth / 4 - pBtn->getWidth() / 2, yPxl);
    pDoc->addComponent(pBtn);

    // Init popup
    pDoc->setDimensions(iWidth, yPxl + pBtn->getHeight() + 10);
    pPopup->setDocument(pDoc);

    guiFrameEffect * pEffect = new guiFrameMouseFocus(POPUP_EFFECT_FOCUS, 1.0f);
    pPopup->addEffect(pEffect);

    pEffect = new EffectComeIn(POPUP_EFFECT_INTRO, 0.3f);
    pPopup->addEffect(pEffect);
    pEffect->reset();

    return pPopup;
}

// -----------------------------------------------------------------
// Name : createTimedPopup
//  Static default constructor
// -----------------------------------------------------------------
guiPopup * guiPopup::createTimedPopup(string sText, double fTimer, int iWidth)
{
    // Create empty popup (frame)
    guiPopup * pPopup = new guiPopup();
    Texture * frmtex[8];
    frmtex[0] = _tex->findTexture("gui/interface:FrmTL");
    frmtex[1] = _tex->findTexture("gui/interface:FrmTC");
    frmtex[2] = _tex->findTexture("gui/interface:FrmTR");
    frmtex[3] = _tex->findTexture("gui/interface:FrmCL");
    frmtex[4] = _tex->findTexture("gui/interface:FrmCR");
    frmtex[5] = _tex->findTexture("gui/interface:FrmBL");
    frmtex[6] = _tex->findTexture("gui/interface:FrmBC");
    frmtex[7] = _tex->findTexture("gui/interface:FrmBR");
    pPopup->init(
        FP_Floating,
        FB_FitFrameToDocument,
        FB_FitFrameToDocument,
        0, 0, 0, 0, frmtex, "TimedPopup", 0, 0, 1, 1);

    // Create popup inner document
    guiDocument * pDoc = new guiPopupDocument();
    pDoc->init(
        "TimedPopupDocument",
        _tex->findTexture("gui/interface:WinBg"),
        0, 0, iWidth, 1);

    // Top label
    guiLabel * pLbl = new guiLabel();
    pLbl->init(sText, H2_FONT, H2_COLOR, "TopLabe", 0, 0, iWidth - 10, 0);
    pLbl->moveTo((iWidth - pLbl->getWidth()) / 2, 10);
    pDoc->addComponent(pLbl);

    // Init popup
    pDoc->setDimensions(iWidth, pLbl->getHeight() + 20);
    pPopup->setDocument(pDoc);

    pPopup->m_fTimer = fTimer;
    return pPopup;
}
