#import "MGLMapViewTests.h"

@interface MGLLayerTests : MGLMapViewTests

@end

@implementation MGLLayerTests

- (void)testDuplicateLayers {
    //Add a source
    MGLVectorSource *source = [[MGLVectorSource alloc] initWithIdentifier:@"my-source" URL:[NSURL URLWithString:@"mapbox://mapbox.mapbox-terrain-v2"]];
    [self.mapView.style addSource: source];
    
    //Prepare layers
    MGLFillStyleLayer *layer1 = [[MGLFillStyleLayer alloc] initWithIdentifier:@"my-layer" source:source];
    MGLFillStyleLayer *layer2 = [[MGLFillStyleLayer alloc] initWithIdentifier:@"my-layer" source:source];
    
    //Add initial layer
    [self.mapView.style addLayer:layer1];
    
    //Try to add the duplicate
    XCTAssertThrowsSpecific([self.mapView.style addLayer:layer2], NSException);
}

@end
