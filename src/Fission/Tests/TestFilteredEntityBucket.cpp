#include <UnitTest++/UnitTest++.h>

#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/Component.h"
#include "Fission/Core/FilteredEntityBucket.h"

#include "Fission/Tests/TestComponent.h"

using namespace fsn;

TEST(FilteredEntityBucket_AllOneEntity)
{
    EntityManager em;

    FilteredEntityBucket bucket;
    bucket.all<TestComponent>();
    em.addEntityObserver(&bucket);

    auto entity = em.createEntityRef(em.createEntity());
    entity.addComponent<TestComponent>();

    CHECK(bucket.getActiveEntities().size() == 1);
    CHECK(bucket.getActiveEntities()[0] == entity);
}

TEST(FilteredEntityBucket_AllTwoEntities)
{
    EntityManager em;

    FilteredEntityBucket bucket;
    bucket.all<TestComponent, Test2Component>();
    em.addEntityObserver(&bucket);

    auto entityA = em.createEntityRef(em.createEntity());
    entityA.addComponent<TestComponent>();
    entityA.addComponent<Test2Component>();

    auto entityB = em.createEntityRef(em.createEntity());
    entityB.addComponent<TestComponent>();
    entityB.addComponent<Test2Component>();

    CHECK(bucket.getActiveEntities().size() == 2);
    CHECK(bucket.getActiveEntities()[0] == entityA);
    CHECK(bucket.getActiveEntities()[1] == entityB);
}

TEST(FilteredEntityBucket_AllTwoEntitiesRemoveComponent)
{
    EntityManager em;

    FilteredEntityBucket bucket;
    bucket.all<TestComponent, Test2Component>();
    em.addEntityObserver(&bucket);

    auto entityA = em.createEntityRef(em.createEntity());
    entityA.addComponent<TestComponent>();
    entityA.addComponent<Test2Component>();

    auto entityB = em.createEntityRef(em.createEntity());
    entityB.addComponent<TestComponent>();
    entityB.addComponent<Test2Component>();

    entityA.removeComponent<TestComponent>();

    CHECK(bucket.getActiveEntities().size() == 1);
    CHECK(bucket.getActiveEntities()[0] == entityB);
}
