#include "WidgetsVRFSA.h"
#include "tester_unreal_VR.h"
#include "ik_logger_unreal.h"
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

void SPanelTextWidget::UpdateInstruction(const FString& info)
{
	m_textBox->SetText(FText::FromString(info));
	LOGIKVar(LogInfoWCharPtr, *info);
}

void SPanelTextWidget::Construct(const FArguments& InArgs)
{
	SAssignNew(m_textBox, STextBlock);
	SUserWidget::Construct( SUserWidget::FArguments()
		[
			// SNew(STextBlock)
			// .Text( NSLOCTEXT("x", "x", "My Widget's Content") )
			// .Text(FText::FromString(FString("Implementation Details!\nHello,world_0\nHello,world_1\nHello,world_2\nHello,world_3\n" )))
			m_textBox.ToSharedRef()
		]);
}

TSharedRef<SPanelTextWidget> SPanelTextWidget::New()
{
	return MakeShareable(new SPanelTextWidget());
}

void SWindowVRFSA::Construct(const FArguments& InArgs)
{
	SWindow::Construct(InArgs);
	SAssignNew(m_paneInstruction, SPanelTextWidget); 
	SetContent(m_paneInstruction.ToSharedRef());
}

void SWindowVRFSA::UpdateInstruction(const FString& info)
{
	m_paneInstruction->UpdateInstruction(info);
}