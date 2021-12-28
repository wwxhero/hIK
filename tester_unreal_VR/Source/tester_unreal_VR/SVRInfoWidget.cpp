#include "SVRInfoWidget.h"
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

void SVRInfoWidget::Construct(const FArguments& InArgs)
{
    SUserWidget::Construct( SUserWidget::FArguments()
        [
           SNew(STextBlock)
           // .Text( NSLOCTEXT("x", "x", "My Widget's Content") )
           .Text(FText::FromString(FString("Implementation Details!\nHello,world_0\nHello,world_1\nHello,world_2\nHello,world_3\n" )))
        ]);
}

TSharedRef<SVRInfoWidget> SVRInfoWidget::New()
{
	return MakeShareable(new SVRInfoWidget());
}