#pragma once
#include "Widgets/SWindow.h"
#include "Widgets/SUserWidget.h"
/* See SUserWidgetExample
 *
 * SMyWidget.h
 * -----------
 * class SMyWidget : public SUserWidget
 * { 
 *   public:
 *   SLATE_USER_ARGS( SMyWidget )
 *   {}
 *   SLATE_END_ARGS()
 *
 *   // MUST Provide this function for SNew to call!
 *   virtual void Construct( const FArguments& InArgs ) = 0;
 *
 *   virtual void DoSomething() = 0;
 * };
 *
 * SMyWidget.cpp
 * -------------
 * namespace Implementation 
 * {
 *   class SMyWidget : public ::SMyWidget
 *   {
 *     public:
 *     virtual void Construct( const FArguments& InArgs ) override
 *     {
 *        SUserWidget::Construct( SUserWidget::FArguments()
 *        [
 *           SNew(STextBlock)
 *           .Text( NSLOCTEXT("x", "x", "My Widget's Content") )
 *        ]
 *     }
 *     
 *     private:
 *     // Private implementation details can occur here
 *     // without ever leaking out into the .h file!
 *   }
 * }
 *
 * TSharedRef<SMyWidget> SMyWidget::New()
 * {
 *   return MakeShareable( new SMyWidget() );
 * }
 */

class SPanelTextWidget : public SUserWidget
{ 
public:
	SLATE_USER_ARGS( SPanelTextWidget )
		{}
	SLATE_END_ARGS()
	// MUST Provide this function for SNew to call!
	virtual void Construct( const FArguments& InArgs );
	void UpdateInstruction(const FString &info);
private:
	TSharedPtr<STextBlock> m_textBox;
};

class SWindowVRFSA : public SWindow
{
public:
	// MUST Provide this function for SNew to call!
	virtual void Construct( const FArguments& InArgs );

	void UpdateInstruction(const FString& info);
private:
	TSharedPtr<SPanelTextWidget> m_paneInstruction;
};
